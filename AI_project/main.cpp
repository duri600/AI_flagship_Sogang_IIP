#define _CRT_SECURE_NO_WARNINGS
#define _CRTDBG_MAP_ALLOC

#include <stdio.h>
#include "ProcBuffers.h"
#include "sigproc.h"
#include <iostream>
#include "SNMF.h"
#include <stdlib.h>
#include <crtdbg.h>
#include <time.h>
#include "header.h"

#pragma comment(lib,"Winmm.lib")

#ifdef _DEBUG 
#ifndef DBG_NEW 
#define DBG_NEW new (_NORMAL_BLOCK, __FILE__, __LINE__) 
#define new DBG_NEW 
#endif 
#endif  // _DEBUG

using namespace std;

int main()
{
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//_CrtSetBreakAlloc(2129);
	clock_t start, end;
	start = clock();
	int ch, Nframe, i, j;
	ProcBuffers *proc;
	proc = new ProcBuffers();

	//souce & noise training
	proc->Training();

	char file_name[Nch][500];
	double *signal[Nch];

	//frame ¥‹¿ß input
	//2ch mic input
	for (ch = 0; ch < Nch; ch++)
	{
		sprintf(file_name[ch], ".\\input\\S1_ch%d.wav", ch + 1);
		std::cout << "reading " << file_name[ch] << std::endl;
		signal[ch] = wavread(file_name[ch]);
	}

	int input_length = length(signal[1]);
	double **Input;
	Input = new double *[Nch];
	for (int i = 0; i < Nch; i++)
	{
		Input[i] = new double[BufferSize];
		for (int j = 0; j < BufferSize; j++)
		{
			Input[i][j] = 0;
		}
	}

	int iteration = input_length / BufferSize;
	for (Nframe = 0; Nframe <= iteration; Nframe++)
	{
		for (i = 0; i < Nch; i++)
		{
			for (j = 0; j < BufferSize; j++)
			{
				if (Nframe*BufferSize + j > input_length)
				{
					Input[i][j] = 0;
				}
				else
				{
					Input[i][j] = signal[i][Nframe*BufferSize + j + 1];
				}
			}

		}
		proc->Process(Input);

	}
	for (ch = 0; ch < Nch; ch++)
	{
		delete[] Input[ch];
	}
	delete[] Input;

	for (int i = 0; i < Nch; i++)
	{
		free(signal[i]);
	}
	end = clock();
	std::cout << "1frame : " << end - start << "ms" << std::endl;
	delete proc;
}
