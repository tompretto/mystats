1
2
3
4
5
6
7
8
9
10
11
12
13
14
15
16
17
18
19
20
21
22
23
24
25
26
27
28
29
30
31
32
33
34
35
36
37
38
39
40
41
42
43
44
45
46
47
48
49
50
51
52
53
54
55
56
57
58
59
60
61
62
63
64
65
66
67
68
69
70
71
72
73
74
75
76
77
78
79
80
81
82
83
84
85
86
87
88
89
90
91
92
93
94
95
96
97
98
99
100
101
102
103
104
105
106
107
108
109
110
111
112
113
114
115
116
117
118
119
120
121
122
123
124
125
126
127
128
129
130
131
132
133
134
135
136
137
138
139
140
141
142
143
144
145
146
147
148
149
150
151
152
153
154
155
156
157
158
159
160
161
162
163
164
165
166
167
168
169
170
171
172
173
174
175
176
177
178
179
#include<unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#define MULTIPLIER 100 //for timer:/proc/stat provides millisec
main(int argc,char **argv) {
    char    mybuf[1000];
    char    cpubuf[1024];//66 for whole keyword and 79
    char diskbuf[800];
    char    timebuf[20];//
    char    uptime[20];//
    char uptime1[20];
    char ctxt[20];// holds number of contextual switches
    //buffer for cpu data
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
    char    vernum[80];
    char vernum1[80];
    int fd_cpu;//file descriptor for cpuinfo file
    int fd_disk;
    int fd;//file descriptor for version file
    int     fd_boot;//file descriptor for boottime file
    int errs;
    int count;
    int i; //counter
    long int u1,u2;
    int s;//"switch statement"-flag AND (also)kernel processes  
    int u;//user mode processes
    int n;//niced user mode processes
    int seconds;
    int minutes;
    int hours;
    int days;
    int weeks;
 
    char numcpu[3]; //number of cpu's
    char numcpu1[3];
 
     
//get and display CPU type, model and number of processors.
    if ((fd_cpu = open ("/proc/cpuinfo",O_RDONLY))< 0){
        perror("Bad open");
        exit(-1);
    }
    if ((count = read(fd_cpu, &cpubuf[0], sizeof(cpubuf))) > 0) {
         
        for(i=0;i<70;i++)
        {
sscanf(&cpubuf[i],"processor    :%s", &numcpu[0]);
sscanf(&cpubuf[i],"model name   :%s %s %s %s %s",&cpu[0],&cpu1[0],&cpu2[0],&cpu3[0],&cpu4[0]);
        }
 
printf("cpu:    %s %s %s %s %s\n",&cpu[0],&cpu1[0],&cpu2[0],&cpu3[0],&cpu4[0]);
printf("Number of processors (if 0, 0==1):  [%s]\n",&numcpu[0]);
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
        printf("Kernel version:  %s\n", &vernum[0]);
    }
    if(count < 0) {
        perror ("Bad read");
        exit (-1);
    }
 
 
 
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
//
//get and display amount of time cpu as spent in user/system mode, or idle.
//
    if ((fd_cpu = open ("/proc/stat",O_RDONLY))< 0){
        perror("Bad open of /proc/stat");
        exit(-1);
    }
    if ((count = read(fd_cpu, &cpubuf[0], sizeof(cpubuf))) > 0) {
         
    for(i=0;i<sizeof(cpubuf);i++)
    {
    sscanf(&cpubuf[i],"cpu  %s %s %s %s %s ",&cpu[0],&cpu1[0],&cpu2[0],&cpu3[0],&cpu4[0]);
    sscanf(&cpubuf[i],"ctxt %s", &ctxt[0]);
    sscanf(&cpubuf[i],"processes %s", &numcpu[0]);
    }
i=2;
i=atoi(&cpu4[0]);//idle
s=atoi(&cpu3[0]);//kernel
u=atoi(&cpu1[0]);//user mode
n=atoi(&cpu2[0]);//niced user mode
 
u=u+n;//total user mode == user mode++ niced user mode processes
    printf("cpu usage:\n\tIdle:[%d]\n\tUser mode:[%d]\n\tKernel mode[%d]\n",i,u,s);
    printf("Number of processes:  [%s]\n",&numcpu[0]);
    printf("Number of contextual Switches: [%s]\n",&ctxt[0]);
}
    if(count < 0) {
        perror ("Bad read");
        exit (-1);
    }
 
 
//extract and display disk io usage
fd=0;
count=0;
    if ((fd_disk = open ("/proc/diskstats",O_RDONLY))< 0){
        perror("Bad open");
        exit(-1);
    }
    if ((count = read(fd_disk, &diskbuf[0], sizeof(diskbuf))) > 0) {
         
for(i=0;i<sizeof(diskbuf);i++){
 
sscanf(&diskbuf[i],"sda %s %s %s %s %s",&disk[0],&disk1[0],&disk2[0],&disk3[0],&disk4[0]);
    }
    if(count < 0) {
        perror ("Bad read");
        exit (-1);
    }
//  printf("Disk stats:%s %s %s %s\n",&disk1[0],&disk2[0],&disk3[0],&disk4[0]);
 
 
    }
        }
    }
    }
 
}
    