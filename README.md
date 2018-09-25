# BWA-protocol
Protocol for making mpileup files from FASTQ files using BWA

Septermber 25, 2018

Author: Takahiro Maruki

**This package can be downloaded by clicking "Download ZIP" on this web site.**

**Instructions**

1. Compile the programs by typing the following command: <br />
chmod u+x B_Adapters.pl <br />
chmod u+x N_Adapters.pl <br />
g++ -o Make_Scripts_IU Make_Scripts_IU.cpp -lm <br />
g++ -o Make_JobScripts_IU Make_JobScripts_IU.cpp -lm <br />
g++ -o Make_SubmitJobs_IU Make_SubmitJobs_IU.cpp -lm <br />
- These programs are found and compiled in the following directory: <br />
/N/dc2/projects/daphpops/Software_BWA_Protocol/ <br />

2. Make a working directory for the data. <br />
For example, type the following command to make a directory called KAP_DataPrep: <br />
mkdir KAP_DataPrep <br />
