#include <iostream>
#include <cstring>
#include <random>
#include <fstream>
#include <numeric>
#include <cmath>
#include <algorithm>
#include <iomanip>
#include <sys/stat.h>
//#include <sys/types.h>
#include "Block.h"
#include "covLoader.h"
#include "InsertSizeEst.h"
#include "util.h"

using namespace std;

#define MAX_PAIR_NUM		200000
#define MAX_ISIZE			2000

struct region{
	string chrname;
	long int startPos = 0, endPos = 0;
	int num;
};
struct warnregion{
	region interval;
	long int startaln, endaln;
};
struct indicators{
	double covscore[3], indelscore[3], clipscore, insertscore, strandscore, matescore;
};
struct kind{
	int flag[7] = {0, 0, 0, 0, 0, 0, 0};
};
struct sortflag{
	float score;
	int order;
};
class Evaluate{
	public:
	//arguments
		bool covflag = true, indelflag = true, abstrandflag = true, abisizeflag = true, abmateflag = true;
		int minRegsize = 200;
		float exRegFold = 2, minCovFold = 0.5, maxCovFold = 2, minLocRatio = 0.2, minStrandRatio = 0.3, minisizeRatio = 0.3, IsizeSdevFold = 3, minMateRatio = 0.3;
		string outdir = "misEval_out";

		Paras *paras;
		vector<indicators> scores;//indicator[0...5]coverage，indel，clip，insertsize，reversed，mate
		vector<kind> result;
		vector<string> inregions;
		vector<region> evaregions, regions, chrregions, ranregion, fairegion;//evaluated region，auxiliary region，merged region，ranregion，chromosal region
		vector<region> abmate, abstrand, abisize;//mark clump
		faidx_t * fai;
		vector<Base*> baseArr;
		Base* basearr;
		vector<vector<bam1_t*>> alnDataVector;
		vector<bam1_t*> alnDataVec;
		vector<vector<int>> abnormalpos;
		double meaninsertsize, mininsertsize, maxinsertsize, meancov, mincov, maxcov,chimeriCoef=1, randomCoef = 0.2;
		string regionfile, bamFile;

		

		Evaluate(char** argv, faidx_t *fai, Paras *paras);
		virtual ~Evaluate();
		void getinregion(string filename);
		void splitstring(string origin, char flag, string &front, string &after);
		void getchrregions();
		void getchrregions(string fai);
		static bool compareContigLen(region a,region b);
		bool isRegExist(region reg, vector <region> vec);
		void getranregion();
		vector<Base*> initbaseinfo();
		Base* initBaseinfo(region r);
		int loadAlnData();
		bool isabstrandRead(bam1_t *b);
		void getreadsMarkregion(region r);
		void getMultiReads(region r);
		void preData(region r);
		void clearData();
		double *abnormalreads(region r);
		double* getabreadsreatio(region r);//caculate indicators in region
		void getindicator(int s);
		void extracfeature();
		void outputfile();
};


