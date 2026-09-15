#include <stdio.h>
#include <string.h>
#include "task2_hash.h"
#include "task2_count.h"
#include "task2_sum.h"

#pragma pack(push, 1)
struct TASK2_FILE_METADATA {
	char szFileName[32];
	int iFileSize;
	char byHash[4];
	int iSumOfChars;
	char aAlphaCount[26];
};
#pragma pack(pop)

int main(void) {
	struct TASK2_FILE_METADATA meta;
	FILE *in = fopen("pgexam25_test.txt", "rb");

	if (!in) {
		perror("fopen input");
		return 1;
	}

	strncpy(meta.szFileName, "pgexam25_test.txt", 31);
	meta.szFileName[31] = 0;

	Task2_SizeAndSumOfCharacters(in, &meta.iFileSize, &meta.iSumOfChars);
	rewind(in);

	unsigned int hashVal;
	Task2_SimpleDjb2Hash(in, &hashVal);
	for (int i = 0; i < 4; i++) {
		meta.byHash[i] = (hashVal >> (8 * i)) & 0xFF;
	}
	rewind(in);

	Task2_CountEachCharacter(in, meta.aAlphaCount);
	fclose(in);

	FILE *out = fopen("pgexam25_output.bin", "wb");

	if (!out) {
		perror("fopen output");
		return 1;
	}

	fwrite(&meta, sizeof(meta), 1, out);
	fclose(out);
	return 0;
}
