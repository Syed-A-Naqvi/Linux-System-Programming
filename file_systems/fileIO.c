#include<stdio.h>

int fileWrite(const char* data, const char* fname){
    
    FILE* file = fopen(fname, "w+"); // use command: man fopen to view all flags

    if(file == NULL){
        printf("Error opening file.\n");
        return 1;
    }
    else{
        printf("File opened successfully.\n");
        printf("Data being written: %s\n", data);
        fprintf(file, "Data: %s\n", data);
        // fwrite(data, sizeof(char), sizeof(data), file);
        fclose(file);
        return 0;
    }

}

int fileRead(char* buffer, const char* fname){
    
    FILE* file = fopen(fname, "r");

    if(file == NULL){
        printf("Error opening file.\n");
        return 1;
    }
    else{
        printf("File opened successfully.\n");

        // reads file word by word, terminating a single scan when encountering a whitespace or \n         
        // while(fscanf(file, "%s", buffer) != EOF){
        //     printf("%s ", buffer);
        // }

        fclose(file);

        return 0;
    }

    // continue from 48:00

}

int main(int argc, char const *argv[])
{
    char data[] = "Ontario Tech University.";
    char buffer[100];
    char fname[] = "./output.txt";

    fileWrite(data, fname);
    fileRead(buffer, fname);
    
    return 0;
}
