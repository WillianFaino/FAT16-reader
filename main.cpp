#include <bits/stdc++.h>
#include "FileSystem.h"

int main(int argc, char* argv[]){
	
	char fileName[1000];
	
	printf("Enter the name of the image to be read: ");
	scanf("%s", &fileName);
	
	FileSystem fatimg("floppyfat2.img");
	
	fatimg.open();
	
	printf("\n\n\n\n----------BOOT RECORD INFO----------\n\n\n\n");
	fatimg.printImgInfo();
	
	
	printf("\n\n\n\n----------START OF FAT, ROOT DIR AND DATA REGION----------\n\n\n\n");
	fatimg.startSectors();
	
	
	printf("\n\n\n\n----------ROOT DIR |- VALID ENTRIES INFO----------\n\n\n\n");
	fatimg.rootDirInfo();
	
	printf("\n\n\n\n----------FILE CONTENT----------\n\n\n\n");
	fatimg.showFileContent();

    return 0;

}
