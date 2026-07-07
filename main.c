#include <stdio.h>
#include <string.h>
void removePathLine(const char *targetFileName, const char *pathToRemove) {
  FILE *fileToRead = fopen(targetFileName, "r");
  if (fileToRead == NULL)
    return;

  FILE *tempFile = fopen("/home/asterion/.temp_zshrc", "w");
  if (tempFile == NULL) {
    fclose(fileToRead);
    return;
  }

  char fileBuffer[BUFSIZ];
  long currentLine = 0;

  while (fgets(fileBuffer, BUFSIZ, fileToRead) != NULL) {
    if (strstr(fileBuffer, pathToRemove) == NULL) {
      fputs(fileBuffer, tempFile);
    }
    currentLine++;
  }

  fclose(tempFile);
  fclose(fileToRead);

  printf("Scanned %ld lines. Removing path.\n", currentLine);

  remove(targetFileName);
  rename("/home/asterion/.temp_zshrc", targetFileName);
}
void addPath(FILE *filePtr, char pathToAdd[256]) {
  fprintf(filePtr, "export PATH=$PATH:%s\n", pathToAdd);
}

void savePathData(char *pathName, char *pathValue) {
  FILE *dataFile = fopen("/home/asterion/projects/CLI_Tool/data.txt", "a+");
  if (dataFile == NULL) {
    dataFile = fopen("/home/asterion/projects/CLI_Tool/data.txt", "w");
  }

  char nameSeparator[2] = {' ', '\0'};
  strcat(pathName, nameSeparator);
  fprintf(dataFile, pathName);

  char lineSeparator[2] = {'\n', '\0'};
  strcat(pathValue, lineSeparator);
  fprintf(dataFile, pathValue);

  fclose(dataFile);
}

const char *findPath(char pathName[25]) {
  FILE *dataFile = fopen("/home/asterion/projects/CLI_Tool/data.txt", "a+");
  rewind(dataFile);

  char *currentToken;
  char fileBuffer[60];
  char *tokenizerState;

  while (fgets(fileBuffer, 60, dataFile) != NULL) {
    currentToken = strtok_r(fileBuffer, " \n", &tokenizerState);

    if (currentToken != NULL && strcmp(currentToken, pathName) == 0) {
      currentToken = strtok_r(NULL, " \n", &tokenizerState);
      break;
    }
  }

  fclose(dataFile);
  return currentToken;
}

void listAllPaths() {
  FILE *dataFile = fopen("/home/asterion/projects/CLI_Tool/data.txt", "r");
  if (dataFile == NULL) {
    printf("No paths found (file is empty or doesn't exist).\n");
    return;
  }

  char fileBuffer[60];
  printf("\n--- All Saved Paths ---\n");

  while (fgets(fileBuffer, 60, dataFile) != NULL) {
    printf("%s", fileBuffer);
  }
  printf("------------------------\n");

  fclose(dataFile);
}

int main() {
  FILE *zshrcPtr = fopen("/home/asterion/.zshrc", "a+");
  if (zshrcPtr == NULL) {
    printf("File is non existent!\n");
    return -1;
  }

  char targetPath[256];
  char pathData[256];

  int choice;

  printf("--- Path Manager ---\n");
  printf("1. Add path\n");
  printf("2. List all paths\n");
  printf("3. Remove path\n");
  printf("-----------------\n");
  printf("Select: ");

  scanf("%d", &choice);

  switch (choice) {
  case 1:
    printf("Enter the name of the path: ");
    scanf("%s", targetPath);
    printf("\nEnter your path: ");
    scanf("%s", pathData);
    savePathData(targetPath, pathData);
    addPath(zshrcPtr, pathData);
    printf("Path added successfully!\n");
    break;

  case 2:
    listAllPaths();
    break;

  case 3:
    printf("\nEnter your path: ");
    scanf("%s", pathData);
    removePathLine("/home/asterion/.zshrc", pathData);
    break;

  default:
    printf("Invalid choice.\n");
    break;
  }

  fclose(zshrcPtr);
  return 0;
}
