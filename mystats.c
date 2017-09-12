#include<unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#define MULTIPLIER 100 //for timer:/proc/stat provides millisec


main(int argc,char **argv) {
	char	mybuf[512];
	
	char 	timebuf[20];//
	char    uptime[20];//
	char uptime1[20];

	char disk[12];
	char disk1[12];
 	char disk2[12];
        char disk3[12];
	char disk4[12];
	char disk5[12];
	char numcpu[3]; //number of cpu's
	char	vernum[80];
	char vernum1[80];
	char cpu[12];
	char cpu1[12];
 	char cpu2[12];
        char cpu3[12];
	char cpu4[12];
	char cpu5[12];

	int fd;
	int cpus;//number of cpus in machine
	int s;
	int seconds;
	int minutes;
	int hours;
	int days;
	int weeks;
	int	fd_cpu;//file descriptor for cpuinfo file
	char 	cpubuf[512];//66 for whole keyword and 79
	int 	fd_boot;//file descriptor for boottime file
	int	errs;
	int	count;
	int i; //counter
	long int u1,u2;

	printf("Welcome to Tom Pretto's Hardware Status Program (mystats)!\n");

//get and display CPU type, model and number of processors.
	if ((fd_cpu = open ("/proc/cpuinfo",O_RDONLY))< 0){
		perror("Bad open");
		exit(-1);
	}
	if ((count = read(fd_cpu, &cpubuf[0], sizeof(cpubuf))) > 0) {
		
		for(i=0;i<70;i++)
		{
			sscanf(&cpubuf[i],"processor	:%s", &numcpu[0]);
			sscanf(&cpubuf[i],"model name	:%s %s %s %s %s",&cpu[0],&cpu1[0],&cpu2[0],&cpu3[0],&cpu4[0]);
		}

		printf("cpu:	%s %s %s %s %s\n",&cpu[0],&cpu1[0],&cpu2[0],&cpu3[0],&cpu4[0]);

		cpus=atoi(&numcpu[0]);
		printf("Number of processors:  [%d]\n",++cpus);
	}
	if(count < 0) {
		perror ("Bad read");
		exit (-1);
	}


//extract and display kernel version
	if ((fd = open ("/proc/version",O_RDONLY))< 0){
		perror("Bad open");
		exit(-1);
	}
	while ((count = read(fd, &mybuf[0], sizeof(mybuf))) > 0) {
		sscanf(&mybuf[0],"Linux version  %s",&vernum[0]);
	}
	
	if(count < 0) {
		perror ("Bad read");
		exit (-1);
	}
	else(printf("Kernel version:  %s\n", &vernum[0]));


//get and display time since boot
	if ((fd_boot = open ("/proc/uptime",O_RDONLY))< 0){
		perror("Bad open");
		exit(-1);
	}
        if(count < 0) {
                perror ("Bad read");
                exit (-1);
	}

	if ((count = read(fd_boot, &timebuf[0], sizeof(timebuf)) ) > 0) 
	{
		sscanf(&timebuf[0],"%s %s",&uptime[0],&uptime1[0]);
        }

	seconds=atoi(&uptime[0]);
	days=seconds/86400;//millisecond conversion already built in!(86400*100)
	hours=((seconds % 86400)/3600);//millisecond conversion built in
	minutes=((seconds%3600)/60);
	seconds = (seconds %60);
        printf("System up for %d:%.2d:%.2d:%.2d\n",days,hours,minutes,seconds);

//Finally, check for command line input
	if (argc == 2) {
		if ( (s=  getopt(argc,argv, "s"))!=-1) { 
		switch(s){
			case 's':
				displayExtendedStats();
				break;
				
			default:
				break;
			}//end switch
		}//end if
	}//end if agc==2
}//end of main



displayExtendedStats(){
	char ctxt[12];// holds number of contextual switches
	char ctxt1[12];

	//buffer for cpu data
	char numcpu[3];
	char numcpu1[3];

	char cpubuf1[100];
	char diskbuf[100];

	char cpu[12];
	char cpu1[12];
 	char cpu2[12];
	char cpu3[12];
	char cpu4[12];
	char cpu5[12];
	
	char disk[12];
	char disk1[12];
 	char disk2[12];
	char disk3[12];
	char disk4[12];
	char disk5[12];
	
	int s;//"switch statement"-flag AND (also)kernel processes	
	int u;//user mode processes
	int n;//niced user mode processes
	int cpus;
	
	int	fd_cpu;//file descriptor for cpuinfo file
	int fd_disk;
	int	fd;//file descriptor for version file
	int	count1;
	int count2;
	int i;
	int context, context2;
//
//get and display amount of time cpu as spent in user/system mode, or idle.
//
	//printf("opening /proc/stat/\n");//DEBUG
	if ((fd_cpu = open ("/proc/stat",O_RDONLY))< 0){
		perror("Bad open of /proc/stat");
		exit(-1);
	}
	//printf("Reading /proc/stat/\n");//DEBUG
	while ((count1 = read(fd_cpu, &cpubuf1[0], sizeof(cpubuf1))) > 0) {
		
		for(i=0;i<(count1-1);i++)
		{
			sscanf(&cpubuf1[i],"ctxt %s ",&ctxt[0]);
			sscanf(&cpubuf1[i],"cpu  %s %s %s %s %s ",&cpu[0],&cpu1[0],&cpu2[0],&cpu3[0],&cpu4[0]);
			sscanf(&cpubuf1[i],"ctxt %s ",&ctxt[0]);
			sscanf(&cpubuf1[i],"processes %s %s ", &numcpu[0]);
		}
	}
	if(count1 < 0) {
		perror ("Bad read");
		exit (-1);
	}

	//printf("Finished Reading /proc/stat/\n");//DEBUG

	i=2;
	i=atoi(&cpu4[0]);//idle
	s=atoi(&cpu3[0]);//kernel
	u=atoi(&cpu1[0]);//user mode
	n=atoi(&cpu2[0]);//niced user mode
	u=u+n;//total user mode == user mode++ niced user mode processes

	context = atoi(&ctxt[0]);//number of contextual shifts
	//printf("finished calculations\n");//DEBUG
	
	printf("cpu usage:\n\tIdle:[%d]\n\tUser mode:[%d]\n\tKernel mode[%d]\n",i,u,s);
	printf("Number of processes:  [%d]\n",atoi(&numcpu[0]));
	printf("Number of contextual Switches: %d \n",context);

	


	//extract and display disk io usage
	if ((fd_disk = open ("/proc/diskstats",O_RDONLY))< 0){
		perror("Bad open");
		exit(-1);
	}
	while ((count2 = read(fd_disk, &diskbuf[0], sizeof(diskbuf))) > 0) {
		
		for(i=0;i<sizeof(count2);i++){

			sscanf(&diskbuf[i],"%s %s sda %s %s %s %s",&disk[0],&disk1[0], &disk2[0],&disk3[0],&disk4[0],&disk5[0]);
		//printf("([%s]::[%s]::[%s]::[%s]::[%s]::[%s])#",&disk[0],&disk1[0], &disk2[0],&disk3[0],&disk4[0],&disk5[0]);//DEBUG
		}
	}
	if (count2 < 0) {
		perror ("Bad read");
		exit (-1);
	}
	printf("Disk stats:\n\tNumber of disk reads:%d\n",atoi(&disk2[0]));
}

