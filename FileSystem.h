#include <bits/stdc++.h>

using namespace std;

//struct que define o formato do boot record.
struct bootSector{
	
	int ftb;
	
	unsigned long long int oemID;
	
	uint16_t bytesPerSector;
	
	uint8_t sectorsPerCluster;
	
	uint16_t nReservedSectors;
	
	uint8_t nTables;
	
	uint16_t nDirectoryEntries;
	
	uint16_t totalSectorsInLogVal; 
	
	uint8_t mediaDescriptorType;
	
	uint16_t nSectorsPerFAT;
	
	uint16_t nSectorsPerTrack;
	
	uint16_t nHeads;
	
	int nHiddenSectors;
	
	int largeSectorCount;
	
};typedef struct bootSector bootSector;

bootSector bootRecord;

class FileSystem
{
	private: bootSector BS;
	private: int tamCluster;
	private: int entradasPorCluster;
	private: int qtdClustersUse;
	private: int tamImg;
	private: int ttlClusters;
	private: char* filename;
	
	private: int dirAtual = 0;
	
	FILE *img;
	
	
	public: FileSystem(char *fname){
		filename = fname;
	}
	
	private: unsigned long long int mergeBytes(uint8_t* br, int sta, int end){
		unsigned long long int temp=0;
		unsigned long long int mult=1;
		
		
		for (int i=sta;i<=end;i++){
			temp += br[i]*mult;
			///printf("br = %u\n", br[i]);
			mult *= 256; 
		}
		
		return temp;
	}
	
	private: void setBSInfo(uint8_t* br){
		BS.ftb = mergeBytes(br, 0, 2);
		BS.oemID = mergeBytes(br, 3, 10);
		BS.bytesPerSector = mergeBytes(br, 11, 12);
		BS.sectorsPerCluster = mergeBytes(br, 13, 13);
		BS.nReservedSectors = mergeBytes(br, 14, 15);
		BS.nTables = mergeBytes(br, 16, 16);
		BS.nDirectoryEntries = mergeBytes(br, 17, 18);
		BS.totalSectorsInLogVal = mergeBytes(br, 19, 20);
		BS.mediaDescriptorType = mergeBytes(br, 21, 21);
		BS.nSectorsPerFAT = mergeBytes(br, 22, 23);
		BS.nSectorsPerTrack = mergeBytes(br, 24, 25);
		BS.nHeads = mergeBytes(br, 26, 27);
		BS.nHiddenSectors = mergeBytes(br, 28, 31);
		BS.largeSectorCount = mergeBytes(br, 32, 35);
	}
	
	public: void printImgInfo(){
		printf("FTB = %u\n", BS.ftb);
		printf("OEM Identifier = %llu\n", BS.oemID);
		printf("Bytes per Sector = %u\n", BS.bytesPerSector);
		printf("Sectors per Cluster: = %u\n", BS.sectorsPerCluster);		
		printf("Number of Reserved Sectors: = %u\n", BS.nReservedSectors);		
		printf("Number of FATs: = %u\n", BS.nTables);		
		printf("Number of Directory Entries: = %u\n", BS.nDirectoryEntries);		
		printf("Total Sectors in Logical Volume: = %u\n", BS.totalSectorsInLogVal);		
		printf("Media Descriptor Type: = %u\n", BS.mediaDescriptorType);		
		printf("Number of Sectors per FAT: = %u\n", BS.nSectorsPerFAT);		
		printf("Number of Sectors per Track: = %u\n", BS.nSectorsPerTrack);		
		printf("Number of head on storage media: = %u\n", BS.nHeads);		
		printf("Number of hidden sectors: = %u\n", BS.nHiddenSectors);		
		printf("Large sector count: = %u\n", BS.largeSectorCount);		
	}

	public: void open(){
		
		uint8_t* br = (uint8_t*)malloc(36);
		img = fopen(filename,"r+");
		fseek(img,0,SEEK_SET);
		fread(br,36,1,img);
		
		fseek(img,0,SEEK_END);
		tamImg=ftell(img);
		fclose(img);
		/*for (int i=0;i<36;i++){
			printf("%x ", br[i]);
		}*/
		
		setBSInfo(br);
	}
	
	public: void startSectors(){
		unsigned long long int startOfFatRegion = BS.nReservedSectors;
		unsigned long long int startOfRootDir = startOfFatRegion+(BS.nTables*BS.nSectorsPerFAT);
		unsigned long long int startOfDataRegion = startOfRootDir + ((BS.nDirectoryEntries*32)+(BS.bytesPerSector-1))/BS.bytesPerSector;
		
		printf("Start of FAT Region: sector %llu, byte no. %llu\n", startOfFatRegion, startOfFatRegion*BS.bytesPerSector);
		printf("Start of Root Directory: sector %llu, byte no. %llu\n", startOfRootDir, startOfRootDir*BS.bytesPerSector);
		printf("Start of Data Region: sector %llu, byte no. %llu\n", startOfDataRegion, startOfDataRegion*BS.bytesPerSector);
	}
	
	public: void rootDirInfo(){
		unsigned long long int sord = BS.nReservedSectors+(BS.nTables*BS.nSectorsPerFAT);
		unsigned long long int rootLen = (((BS.nDirectoryEntries*32)+(BS.bytesPerSector-1))/BS.bytesPerSector)*BS.bytesPerSector;
		sord*=BS.bytesPerSector;
		
		uint8_t* entry = (uint8_t*)malloc(32);
		img = fopen(filename,"r+");
		fseek(img,sord,SEEK_SET);
		for (int i=0;i<(rootLen/32);i++){
			fread(entry,32,1,img);
			char name[9];
			char ext[4];
			uint8_t attr;
			uint16_t firstClu;
			unsigned int fileSize;
			name[0]=entry[0];
			name[1]=entry[1];
			name[2]=entry[2];
			name[3]=entry[3];
			name[4]=entry[4];
			name[5]=entry[5];
			name[6]=entry[6];
			name[7]=entry[7];
			name[8]=0;
			ext[0]=entry[8];
			ext[1]=entry[9];
			ext[2]=entry[10];
			ext[3]=0;
			attr = mergeBytes(entry, 11, 11);
			firstClu = mergeBytes(entry,26,27);
			fileSize = mergeBytes(entry, 28, 31);
			if(attr==15 || (fileSize==0 && attr!=16)) continue;
			printf("File name: %s\n", name);
			printf("File extension: %s\n", ext);
			printf("File attribute: %u\n", attr);
			printf("File first cluster: %u\n", firstClu);
			printf("File size: %u\n", fileSize);
			cout<<"\n";
		}
		
		
		
	}
	
	public: void showFileContent(){
		unsigned long long int startOfFatRegion = BS.nReservedSectors;
		unsigned long long int startOfRootDir = startOfFatRegion+(BS.nTables*BS.nSectorsPerFAT);
		unsigned long long int startOfDataRegion = startOfRootDir + ((BS.nDirectoryEntries*32)+(BS.bytesPerSector-1))/BS.bytesPerSector;
		char na[9];
		char ex[4];
		char thisAll[13];
		
		printf("Enter the name of the file to view its content (include extension, if not a directory): ");
		
		scanf("%s", thisAll);
		printf("\n\n\n\n");
		int j=0;
		bool flag=false;
		for (int i=0;i<strlen(thisAll)+1;i++){
			if(thisAll[i]=='.'){
				na[i]=0;
				flag=true;
				continue;
			}
			if(thisAll[i]==0){
				if(flag){
					ex[j]=0;
				}else{
					na[i]=0;
					ex[j]=0;
				}
				break;
			}
			if(!flag){
				na[i] = thisAll[i];
			}else{
				ex[j] = thisAll[i];
				j++;
			}
		}
		
		
		
		
		
		unsigned long long int sord = BS.nReservedSectors+(BS.nTables*BS.nSectorsPerFAT);
		unsigned long long int rootLen = (((BS.nDirectoryEntries*32)+(BS.bytesPerSector-1))/BS.bytesPerSector)*BS.bytesPerSector;
		sord*=BS.bytesPerSector;
		
		uint8_t* entry = (uint8_t*)malloc(32);
		img = fopen(filename,"r+");
		fseek(img,sord,SEEK_SET);
		for (int i=0;i<(rootLen/32);i++){
			fread(entry,32,1,img);
			char name[9];
			char ext[4];
			uint8_t attr;
			uint16_t firstClu;
			unsigned int fileSize;
			name[0]=entry[0];
			name[1]=entry[1];
			name[2]=entry[2];
			name[3]=entry[3];
			name[4]=entry[4];
			name[5]=entry[5];
			name[6]=entry[6];
			name[7]=entry[7];
			name[8]=0;
			ext[0]=entry[8];
			ext[1]=entry[9];
			ext[2]=entry[10];
			ext[3]=0;
			for (int i=0;i<strlen(name);i++){
				if(name[i]==32){
					name[i] = 0;
					break;
				}
			}
			for (int i=0;i<strlen(ext);i++){
				if(ext[i]==32){
					ext[i] = 0;
					break;
				}
			}
			attr = mergeBytes(entry, 11, 11);
			firstClu = mergeBytes(entry,26,27);
			fileSize = mergeBytes(entry, 28, 31);
			if (strcasecmp(na, name) == 0){
				if (strcasecmp(ex, ext) == 0){
					int clusterSt = ((firstClu-2)*BS.sectorsPerCluster)+startOfDataRegion;
					
					clusterSt*=BS.bytesPerSector;
					
					if(attr==16){
						uint8_t* entry = (uint8_t*)malloc(32);
						img = fopen(filename,"r+");
						fseek(img,clusterSt,SEEK_SET);
						for (int i=0;i<(BS.bytesPerSector*BS.sectorsPerCluster)/32;i++){
							fread(entry,32,1,img);
							char name[9];
							char ext[4];
							uint8_t attr;
							uint16_t firstClu;
							unsigned int fileSize;
							name[0]=entry[0];
							name[1]=entry[1];
							name[2]=entry[2];
							name[3]=entry[3];
							name[4]=entry[4];
							name[5]=entry[5];
							name[6]=entry[6];
							name[7]=entry[7];
							name[8]=0;
							ext[0]=entry[8];
							ext[1]=entry[9];
							ext[2]=entry[10];
							ext[3]=0;
							attr = mergeBytes(entry, 11, 11);
							firstClu = mergeBytes(entry,26,27);
							fileSize = mergeBytes(entry, 28, 31);
							if(attr==15 || (fileSize==0 && attr!=16)) continue;
							printf("File name: %s\n", name);
							printf("File extension: %s\n", ext);
							printf("File attribute: %u\n", attr);
							printf("File first cluster: %u\n", firstClu);
							printf("File size: %u\n", fileSize);
							cout<<"\n";
						}
					}else{
						uint16_t newClu = firstClu;
						while(true){
							char* entry = (char*)malloc(BS.bytesPerSector*BS.sectorsPerCluster);
							img = fopen(filename,"r+");
							fseek(img,clusterSt,SEEK_SET);
							fread(entry,1,BS.bytesPerSector*BS.sectorsPerCluster,img);
							for (int i=0;i<(BS.bytesPerSector*BS.sectorsPerCluster);i++){
								printf("%c",entry[i]);
							}
							int pos = (startOfFatRegion*BS.bytesPerSector)+(newClu*2);
							
							fseek(img,pos,SEEK_SET);
							fread(&newClu,2,1,img);
							
							clusterSt = ((newClu-2)*BS.sectorsPerCluster)+startOfDataRegion;
							clusterSt*=BS.bytesPerSector*BS.sectorsPerCluster;
							if(newClu>65534){
								break;
							}
						}
					}
				}
			}
		}
	}
	
};
