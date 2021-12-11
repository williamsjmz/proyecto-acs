#include <stdio.h>

int attend_request(char * command[], char * file_name, char * response)
{

    FILE *file;                 // Variable apuntador al archivo
    char content[50];          // Variable para el contenido del archivo

    // Si el comando es INSERT
    if ((strcmp(command[0], "INSERT") == 0) || (strcmp(command[0], "insert") == 0))
    {
        // Copia el contenido del archivo
        strcpy(content, command[2]);

        // Crea el archivo con el número de cuenta como nombre y escribe el nombre del alumno en él
        file = fopen(file_name, "w");

        // Si el archivo no se abrió
        if (file == NULL)
        {
            return 2;
        }
        else
        // Si el archivo se abre correctamente
        {
            fprintf(file, "%s", content);

            fflush(stdout);
            fflush(file);

            fclose(file);

            return 0;
        }
    }
    // Si el comando es SELECT
    else if ((strcmp(command[0], "SELECT") == 0) || (strcmp(command[0], "select") == 0))
    {

        // Abre el archivo en modo lectura
        file = fopen(file_name, "r");

        // Si el archivo no se abrió
        if (file == NULL)
        {
            return 2;
        }
        // Si el archivo se abre correctamente
        else 
        {
            // Lee el contenido y lo almacena el el arreglo command
            fgets(response, 100, file);
            return 1;
        }
        
    }
    // Si por alguna razón no es ninguno de los dos
    else
    {
        return 2;
    }

}

// Valida el comando introducido por el usuario
int validate_command(char * command_pieces[])
{
    if (((strcmp(command_pieces[0], "INSERT") == 0) || (strcmp(command_pieces[0], "SELECT") == 0) || (strcmp(command_pieces[0], "insert") == 0) || (strcmp(command_pieces[0], "select") == 0)) && ((int*)command_pieces[1] > 0)) {
        return 1;
    } else {
        return 0;
    }
}


// Divide una cadena de caracteres en 3 piezas
void split_string(char * inputString, char * pieces[])
{
    int indexCtr = 0;
    int wordIndex = 0;
    int totalWords = 0;
    char words[3][50];

    // Loop through each character in the string
    for (indexCtr = 0; indexCtr <= strlen(inputString); indexCtr++) 
    {
        // If current character is space or null
        if (inputString[indexCtr] == ' ' || inputString[indexCtr] == '\0')
        {
            // If the first two words have already been copied
            if (totalWords >= 2)
            {
                // If it's the end of the string
                if (inputString[indexCtr] == '\0')
                {
                    // Append the null character to the current word
                    words[totalWords][wordIndex] = '\0';
                }
                // If it's not the end of the string
                else
                {
                    // Append a space character to the current word
                    words[totalWords][wordIndex] = ' ';
                    
                    // Increment the index for the word
                    wordIndex++;
                }
            }
            // The first two words are not copied yet
            else
            {
                // Append the null character to the current word
                words[totalWords][wordIndex] = '\0';

                // Append the entire word to the list of pieces
                pieces[totalWords] = words[totalWords];

                // Increment total words found
                totalWords++;

                // Reset the index to zero for the next word
                wordIndex = 0;

            }
        }
        // Found a character of a word
        else
        {
            if (inputString[indexCtr] != '\n')
            {
                // Copy the character of the word
                words[totalWords][wordIndex] = inputString[indexCtr];

                // Increment the index for the word
                wordIndex++;
            }
        }
    }

    // Append the entire word to the list of pieces
    pieces[totalWords] = words[totalWords];
}