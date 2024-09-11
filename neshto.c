#include <stdio.h>
#include <stdlib.h>


typedef struct {
    char name[31];
    char expiration_date[8];
    long long code;
    double price;
    int quantity;
}Medicine;


void discount_price(Medicine *medicine , int count , const char* date) {
    int month , year;
    sscanf(date , "%d.%d" , &month , &year);

    for(int i = 0 ; i < count ; i++) {
        int medicine_month , medicine_year;
        sscanf(medicine[i].expiration_date , "%d.%d" , &medicine_month , &medicine_year);
        if(medicine_year < year || (medicine_year == year && medicine_month < month)){
            double old_price = medicine[i].price;
            medicine[i].price *= 0.8;
            printf("%s-%s - %.2fлева - %.2fлева ", 
            medicine[i].name , medicine[i].expiration_date , old_price , medicine[i].price);
        }
    }
}

void write_binnary(Medicine *medicine , int count , int min_quantity ) {
    FILE *f = fopen("offer.bin", "wb" );
    if(!f) {
        printf("File couldnt be opened");
        return;
    }

    for(int i = 0; i < count ; i++) {
        if(medicine[i].quantity > min_quantity) {
            if(fwrite(medicine[i].name , sizeof(char) , 1 , f)!=1 ||
              fwrite(medicine[i].expiration_date , sizeof(char) , 1 , f)!=1    ||
                fwrite(&medicine[i].code , sizeof(long) , 1 , f)!=1||
                  fwrite(&medicine[i].price , sizeof(double) , 1 , f)!=1||
                    fwrite(&medicine[i].quantity , sizeof(int) , 1 , f)!=1 ){
                        printf("Error writing in file");
                    }

        }
    }
    fclose(f);
}

void delete_medicine(Medicine *medicine , int *count , int code) {
    int found = 0;
    for(int i = 0 ; i < count; i++) {
        if(medicine[i].code == code){
            found = 1;
            for(int j = i; j < count - 1 ; j++){
                medicine[j] = medicine[i + 1];

            }
            (*count)--;
            break;
        }
    }
    if(found){
        medicine = realloc(medicine , (*count) * sizeof(Medicine*));
        printf("Lekarstvo s nomer %lld e iztrito!" , code);


    }else{
        printf("Lekarstvo s nomer %lld ne e namereno!" , code);
    }
}


int main() {
    FILE *file = fopen("medicine.txt" , "r");

    if(!file) {
        printf("Error openning the file");
        return 1;

    }

    int count = 0;
    int capacity = 10;

    Medicine *medicine = (Medicine*)malloc(sizeof(Medicine*) * capacity);
    
    if(!medicine) {
        printf("Memmory allocation failed");
        return 1;

    }

    while(fscanf(file , "%30[^;];%7[^;];%lld;%f;%d" , 
    medicine[count].name , medicine[count].expiration_date ,
     &medicine[count].code , &medicine[count].price , medicine[count].quantity) == 5){
        count++;

        if(count >= capacity) {
            capacity *= 2;
            medicine = realloc(medicine , capacity * sizeof(Medicine*));
            if(!medicine){
                printf("Memory allocation failed");
                fclose(file);
                return 1;
            }
            
        }
     }
     fclose(file);
     free(medicine);


}