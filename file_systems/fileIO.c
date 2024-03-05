#include<stdio.h>

int fileWrite(const char* data, const char* fname){
    
    FILE* file = fopen(fname, "w+"); // use command: man fopen to view all flags

    if(file == NULL){
        printf("Error opening file.\n");
        return 1;
    }
    else{
        printf("File opened successfully for writing.\n");
        printf("Data written:\n\n%s\n", data);
        fprintf(file, "%s", data);
        //for writing bytes instead of text
        // written bytes not always
        // fwrite(data, sizeof(char), sizeof(data), file);
        fclose(file);
        return 0;
    }

}

int fileRead(char* buffer, const char* fname, size_t bufferSize){
    
    FILE* file = fopen(fname, "r");

    if(file == NULL){
        printf("Error opening file.\n");
        return 1;
    }
    else{
        printf("File opened successfully for reading.\n");

        // reads file word by word, terminating a single scan when encountering a whitespace or \n         
        // while(fscanf(file, "%s", buffer) != EOF){
        //     printf("%s ", buffer);
        // }

        // reads file line by line instead of word by word
        // each iteration ends when fgets encounters a \n character
        // printf("Data read:\n\n");
        // while(fgets(buffer, bufferSize, file)){
        //     printf("%s", buffer);            
        // }

        // fseek sets file position indicator to a specified offest from a specified position(beg,mid,end)
        fseek(file, 8, SEEK_SET);
        int bytesread = fread(buffer, sizeof(char), bufferSize-1, file);
        buffer[bytesread] = '\0';
        printf("%s", buffer);
        fclose(file);

        return 0;
    }

}

int main(int argc, char const *argv[])
{
    char data[] = "Ontario Tech University.\nA cool school.\n";
    char buffer[100];
    char fname[] = "./output.txt";

    fileWrite(data, fname);
    printf("\n");
    fileRead(buffer, fname, sizeof(buffer));
    
    return 0;
}
