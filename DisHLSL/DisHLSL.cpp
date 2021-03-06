// DisHLSL.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
//#include <stdio.h>
#include <iostream>
#include <D3Dcompiler.h>


int main(int argc, char* argv[])
{
    FILE *fp;
    FILE *fpOut;
    if ((argc > 2) && 
        argv[1] && (fopen_s(&fp, argv[1], "r+b") == 0) && 
        argv[2] && (fopen_s(&fpOut, argv[2], "w+b") == 0))
    {
        fseek(fp, 0, SEEK_END);
        long fsize = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        PBYTE pFc = (PBYTE)malloc(fsize);
        fread(pFc, 1, fsize, fp);

        ID3DBlob *pShader = NULL;
        LPVOID pShaderDis = NULL;
        size_t hlslSize = 0;

        if (SUCCEEDED(D3DDisassemble(pFc, (size_t)fsize, 0, "// Disassembled by DisHLSL:\n", &pShader)))
        {
            hlslSize = pShader->GetBufferSize();
            pShaderDis = pShader->GetBufferPointer();
        }

        fwrite(pShaderDis, 1, hlslSize, fpOut);
        fclose(fpOut);

        if (pShader)
            pShader->Release();
        free(pFc);
        fclose(fpOut);
        fclose(fp);
    }
    else
    {
        std::cout<<"Usage: DisHLSL.exe <HLSL bytecode> <HLSL disassemble>"<<std::endl;
    }
    return 0;
}

