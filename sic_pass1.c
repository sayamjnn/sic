#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct SymbolTable{
    char label[100];
    int address;
};

struct Token{
    char label[100];
    char opcode[100];
    char operand[100];
};

struct SymbolTable sym[100];
int address;

void tokenise(char line[500],struct Token *token){
    int result = sscanf(line, "%s %s %s", token->label, token->opcode, token->operand);
    if(result==2){
        // char temp[100];
        // strcpy(temp, token->label);
        strcpy(token->operand, token->opcode);
        strcpy(token->opcode, token->label);
        strcpy(token->label, "");
    }else if(result==1){
        strcpy(token->opcode, token->label);
        strcpy(token->label, "");
        strcpy(token->operand, "");
    }
}


int search_opcode(FILE *fp1,struct Token token){
    char line[500];
    struct Token token2;
    while(fgets(line,sizeof(line),fp1)!=NULL){
        tokenise(line, &token2);
        if(strcmp(token.opcode,token2.opcode)==0){
            return 1;
        }
    }
    return 0;
}


int search_label(struct Token token,int n){
    for (int i = 0; i < n;i++){
        if(strcmp(token.label,sym[i].label)==0){
            return 1;
        }
    }
    return 0;
}

int main(){

    struct Token token;
    int temp, i = 0, dup_flag = 0,address,start,Line=0;
    char line[500];

    FILE *input, *output, *fp1, *fp2;
    input = fopen("input2.txt", "r+");
    output = fopen("intermediate.txt", "w+");
    fp1 = fopen("optab.txt", "r+");
    fp2 = fopen("symtab.txt", "w+");

    fgets(line, sizeof(line), input);
    tokenise(line, &token);

    if(strcmp(token.opcode,"START")==0){
        start = address = (int)strtol(token.operand, NULL, 16);
        fprintf(output, "%-10x %-10s %-10s %-10s\n", start, token.label, token.opcode, token.operand);
    }else{
        address = 0;
    }

    while(fgets(line,sizeof(line),input)!=NULL){
        dup_flag = 0;
        temp = address;
        tokenise(line, &token);
        rewind(fp1);
        if(search_opcode(fp1,token)){
            address += 3;
        }else if(strcmp(token.opcode,"RESW")==0){
            address += (3 * atoi(token.operand));
        }
        else if (strcmp(token.opcode, "RESB") == 0){
            address += atoi(token.operand);
        }
        else if (strcmp(token.opcode, "WORD") == 0){
            address += 3;
        }
        else if (strcmp(token.opcode, "BYTE") == 0){
            if(token.operand[0]=='C'){
                address += strlen(token.operand) - 3;
            }else{
                address += (strlen(token.operand) - 3) / 2;
            }
        }else{
            printf("INvalid Opcode at line %d\n", Line);
            dup_flag = 1;
        }
        Line++;
        if(!dup_flag){
            fprintf(output, "%-10x %-10s %-10s %-10s\n", temp, token.label, token.opcode, token.operand);
        }
        if(strcmp(token.label,"")!=0&&!dup_flag){
            if(search_label(token,i)){
                printf("Duplicate Label Found");
            }else{
                fprintf(fp2, "%-10s %-10x\n", token.label, temp);
                sym[i].address = address;
                strcpy(sym[i].label, token.label);
                printf("%s\n", sym[i].label);
                i++;
            }
        }
    }

    fclose(input);
    fclose(output);
    fclose(fp1);
    fclose(fp2);

    printf("Length of the program: %x", address - start);
    return 0;
}
