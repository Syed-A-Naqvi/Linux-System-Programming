#include<stdio.h>
#include<string.h>

// since we are told there are only three lines i am assuming this means there are only
// three '\n' end of line characters and copied the provided text as such

int countString(const char* buffer){

    int wordlen = 0;
    char word[50] = "\0";
    int canCount = 0;

    for (size_t i = 0; i < strlen(buffer)+1; i++){

        char temp[2] = {buffer[i], '\0'};
        strcat(word, temp);
        wordlen++;

        if( buffer[i] < 65 || (buffer[i] > 90 && buffer[i] < 97) || buffer[i] > 122){
            word[wordlen-1] = '\0';
            if (strcmp(word,"Canada") == 0){
                canCount++;
            }
            wordlen = 0;
            word[0] = '\0';
        }
        
    }
    return canCount;
    
}

void fileToString(char fileName[], char buffer[], int bufferSize){
    
    FILE* file = fopen(fileName, "r");
    char tempBuffer[bufferSize];

    if(file == NULL){
        printf("Error opening file.\n");
    }
    else{

        int line = 1;
        while(fgets(tempBuffer, bufferSize, file)){        
            if(line == 2){
                tempBuffer[strlen(tempBuffer)-1] = '\0';
                strcpy(buffer, tempBuffer);
            }
            line++;
        }
        printf("%s\n", buffer);
    }



}

int main(int argc, char const *argv[]){

    // Update fileName with your file name and path
    char fileName[] = "./lab07.txt";
    char buffer[1024];
    printf("\nReading the Second line from the file...\n");
    printf("*******************************************\n");
    fileToString(fileName, buffer, 1024);
    printf("\nNumber of times Canada appeared in the Second line: %d\n\n", countString(buffer));
    return 0;
}
