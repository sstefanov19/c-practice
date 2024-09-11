#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    char name[31];
    char expiration_date[8];
    long long code;
    double price;
    int quantity;
} Medicine;


void apply_discount(Medicine *medicine , int count , const char* date) {
    int month , year;
    sscanf(date , "%d.%d" , &month , &year);

    for(int i = 0 ; i < count ; i++) {
        int med_month , med_year;
        sscanf(medicine[i].expiration_date , "%d.%d" , &med_month, &med_year);
        if(med_year < year || (med_year == year && med_month < month)) {
            double price = medicine[i].price;
           medicine[i].price *= 0.8;
           printf("%s-%s-%.2f лева -> %.2f лева \n"
           , medicine[i].name , medicine[i].expiration_date , 
           price , medicine[i].price);

        }
    }

}

void save_binary_file(Medicine *medicine , int count ,int min_quantity) {
    FILE *f = fopen("offer.bin" , "wb");
    if(!f) {
        printf("Error oppening file");
        return;
    }

    for(int i = 0 ; i < count ; i++) {
        if(medicine[i].quantity > min_quantity) {
            if(fwrite(&medicine[i].price , sizeof(double) , 1 , f)!= 1 ||
            fwrite(&medicine[i].quantity , sizeof(int) , 1 , f) != 1 ) {
                printf("Error writing to file");
                fclose(f);
                return;
            }
        }
    }
    fclose(f);
}

void delete_medicine(Medicine* medicine , int* count ,long long code) {
    int found = 0;
    for(int i = 0; i< *count ; i++ ) {
        if(medicine[i].code == code) {
            found = 1;
            for(int j = i ; j < *count - 1; j++) {
                medicine[j] = medicine[i + 1];

            }
            (*count)--;
            break;
        }
    }

    if(found) {
        medicine = realloc(medicine , (*count) * sizeof(Medicine));
        printf("Lekerstva s kod %lld  bqha iztriti. \n" ,  code);
    }else {
        printf("Nqma namereno lekarstvo s tozi kod %lld ", code);
    }
}




int main()
{

    FILE *file = fopen("medicine.txt", "r");
    if (!file)
    {
        printf("Error oppening file");
        return 0;
    }

    int capacity = 10;
    int count = 0;

    Medicine *medicine = (Medicine *)malloc(capacity * sizeof(Medicine *));
    if (!medicine)
    {
        printf("Memory allocation failed");
        return 1;
    }

    while (fscanf(file, "%30[^;];%7[^;]%lld;%lf;%d\n",
                  medicine[count].name, medicine[count].expiration_date, &medicine[count].code,
                  &medicine[count].price, &medicine[count].quantity) == 5)
    {
        count++;

        if (count >= capacity)
        {
            capacity *= 2;
            medicine = realloc(medicine, capacity * sizeof(Medicine));
        }
    }

    fclose(file);
    free(medicine);

    return 0;
}
