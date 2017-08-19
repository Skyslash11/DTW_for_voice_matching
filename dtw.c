#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

using namespace std;

long long int calculate_distance(short int *sp1,short int *sp2);
long long int find_min(long long int a,long long int b,long long int c);
double calculate_average(short int *sp);
int calculate_distance2(int a,int b);

#define MAX_VALUE 1000000000000000
// WAVE PCM soundfile format (you can find more in https://ccrma.stanford.edu/courses/422/projects/WaveFormat/ )
typedef struct header_file
{
    char chunk_id[4];
    int chunk_size;
    char format[4];
    char subchunk1_id[4];
    int subchunk1_size;
    short int audio_format;
    short int num_channels;
    int sample_rate;			// sample_rate denotes the sampling rate.
    int byte_rate;
    short int block_align;
    short int bits_per_sample;
    char subchunk2_id[4];
    int subchunk2_size;			// subchunk2_size denotes the number of samples.
} header;

typedef struct header_file* header_p;




int main()

{
	FILE * infile1 = fopen("dileepn1.wav","rb");		// Open wave file in read mode
	FILE * infile2 = fopen("ashitan1.wav","rb");	
	FILE * outfile = fopen("Output11.wav","wb");		// Create output ( wave format) file in write mode
	FILE * o1= fopen("sp1.txt","wb");
	FILE * o2 = fopen("sp2.txt","wb");

	int BUFSIZE = 512;					// BUFSIZE can be changed according to the frame size required (eg:512)
	int count = 0;						// For counting number of frames in wave file.
	short int buff16[BUFSIZE];				// short int used for 16 bit as input data format is 16 bit PCM audio
	header_p meta = (header_p)malloc(sizeof(header));	// hea1der_p points to a header struct that contains the wave file metadata fields
	int nb;							// variable storing number of byes returned
	short int ** speech1=(short int **)malloc(sizeof(short int *)*100000);
	short int ** speech2=(short int **)malloc(sizeof(short int *)*100000);
	int frames1=0,frames2=0;
	for(int i=0;i<100000;++i)
	{
		speech1[i]=(short int *)malloc(sizeof(short int)*512);
		speech2[i]=(short int *)malloc(sizeof(short int)*512);
	}
	
	if (infile1)
	{
		fread(meta, 1, sizeof(header), infile1);
		fwrite(meta,1, sizeof(*meta), outfile);
		cout << " Size of Header file is "<<sizeof(*meta)<<" bytes" << endl;
		cout << " Sampling rate of the input wave file is "<< meta->sample_rate <<" Hz" << endl;
		cout << " Number of samples in wave file are " << meta->subchunk2_size << " samples" << endl;

		while (!feof(infile1))
		{
			
			
			nb = fread(buff16,1,BUFSIZE,infile1);	
			printf("%d\n",buff16[0]);
			for(int i=0;i<512;++i)
			{
				fprintf(o1,"%d ",buff16[i]);
				speech1[count][i]=buff16[i];
			}
		    	//fprintf(o1,"\n");
			count++;				
		}
	frames1=count;
	cout << " Number of frames in the input1 wave file are " <<frames1 << endl;
	}
	count=0;
	fclose(o1);
	if (infile2)
	{
		fread(meta, 1, sizeof(header), infile2);
		fwrite(meta,1, sizeof(*meta), outfile);
		cout << " Size of Header file is "<<sizeof(*meta)<<" bytes" << endl;
		cout << " Sampling rate of the input wave file is "<< meta->sample_rate <<" Hz" << endl;
		cout << " Number of samples in wave file are " << meta->subchunk2_size << " samples" << endl;

		while (!feof(infile2))
		{
			
			
			nb = fread(buff16,1,BUFSIZE,infile2);	
			//printf("......%d",buff16[0]);
			for(int i=0;i<512;++i)
			{
				//fprintf(o2,"%d ",buff16[i]);
				speech2[count][i]=buff16[i];
			}
			//fprintf(o2,"\n");
			count++;				
		}
	frames2=count;
	cout << " Number of frames in the input2 wave file are " <<frames2 << endl;
	}
	fclose(o2);
	printf("frames1 = %d\n",frames1);
	printf("frames2 = %d\n",frames2);
	
	long long int dp[frames1][frames2];
	printf("......%d......%d\n",speech1[4][0],speech2[4][0]);
	//Speech Comparision_______________________________________________________________________________

	for(int i=0;i<frames1;++i)
	{
		for(int j=0;j<frames2;++j)
		{
			if(i==0 && j==0)
				dp[i][j]=calculate_distance(speech1[i],speech2[j]);
			else if(i==0 && j!=0)
			{
				dp[i][j]=calculate_distance(speech1[i],speech2[j])+dp[i][j-1];
				if(dp[i][j]/MAX_VALUE >= 1)
					dp[i][j]=MAX_VALUE;
			}
			else if(i!=0 && j==0)
			{
				dp[i][j]=calculate_distance(speech1[i],speech2[j])+dp[i-1][j];
				if(dp[i][j]/MAX_VALUE >= 1)
					dp[i][j]=MAX_VALUE;
			}
			else
			{
				//printf("_______%d\n",speech1[i][0]-speech2[j][0]);
				
				dp[i][j]=calculate_distance(speech1[i],speech2[j])+find_min(dp[i][j-1],dp[i-1][j-1],dp[i-1][j]);
				if(dp[i][j]/MAX_VALUE >= 1)
					dp[i][j]=MAX_VALUE;
			}
		}
	}
	printf("The distance is : %lld\n",dp[frames1-1][frames2-1]);
	printf("This min is : %lld\n",find_min(4,3,1));
	int seq1[frames1],seq2[frames2];
	double prev_avg=0;
	for(int i=0;i<frames1;++i)
	{
		if(i==0)
			prev_avg=calculate_average(speech1[i]);
		double pres_avg=calculate_average(speech1[i]);
		seq1[i]=pres_avg-prev_avg;
		prev_avg=pres_avg;
	}
	for(int i=0;i<frames2;++i)
	{
		if(i==0)
			prev_avg=calculate_average(speech2[i]);
		double pres_avg=calculate_average(speech2[i]);
		if(i<120)
		seq2[i]=pres_avg-prev_avg;
		else
		seq2[i]=0;
		//printf("...%d\n",seq2[i]);
		prev_avg=pres_avg;
	}
	printf("_________________ : %d   %d\n",speech1[2][2],speech2[2][2]);
	int dp2[frames1][frames2];
	for(int i=0;i<frames1;++i)
	{
		for(int j=0;j<frames2;++j)
		{
			if(i==0 && j==0)
				dp2[i][j]=calculate_distance2(seq1[i],seq2[j]);
			else if(i==0 && j!=0)
			{
				dp2[i][j]=calculate_distance2(seq1[i],seq2[j])+dp2[i][j-1];
			}
			else if(i!=0 && j==0)
			{
				dp2[i][j]=calculate_distance2(seq1[i],seq2[j])+dp2[i-1][j];
			}
			else
			{
				dp2[i][j]=calculate_distance2(seq1[i],seq2[j])+find_min(dp2[i][j-1],dp2[i-1][j-1],dp2[i-1][j]);
			}	
		}
	}
	printf("The test result is : %d\n",dp2[frames1-1][frames2-1]);
return 0;
}


long long int calculate_distance(short int *sp1,short int *sp2)
{
	long long int distance=0;
	int a=0,b=0;
	for(int i=0;i<512;++i)
	{
		distance=distance+sqrt(((sp1[i]-sp2[i])/100)*((sp1[i]-sp2[i])/100));
		//a+=sp1[i]/512;
		//b+=sp2[i]/512;
	}	
	//printf("_______%d\n",sp1[0]-sp2[0]);
	return distance;
}

long long int find_min(long long int a,long long int b,long long int c)
{
	long long int min=MAX_VALUE;
	if(a<b && a>=0 && b>=0)
		min=a;
	else if (a>=b && a>=0 && b>=0)
		min=b;
	else
		min=MAX_VALUE;
	if(c<min && c>=0)
		min=c;
	return min;
}

double calculate_average(short int *sp)
{
	double avg=0;
	for(int i=0;i<512;++i)
	{       
		if(sp[i]>0)
		avg=avg+(double)sp[i]/512;
		else
		avg=avg+(double)sp[i]/512;
	}
	return avg;
}

int calculate_distance2(int a,int b)
{
	return (int)sqrt((a-b)*(a-b));
}
