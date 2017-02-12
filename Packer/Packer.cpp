#include "../PackerSample/shared.h"
#include <stdio.h>
#include <memory>
using namespace std;

// Find the PakedBuffer inside the exe file
bool read_pb(FILE *fp, uint64_t marker, PackedBuffer &es)
{
	// size of the bufer for exe
	// is exe is larger than 1MB, and packed value starts on 1st megabyte but ends in another megabyte block,
	// then we have a bug
	const int len = 1024 * 1024;

	unique_ptr<char> buff(new char[len]); // too lazy to deallc manually

	fseek(fp, 0, SEEK_SET);

	while (1)
	{
		int bread = fread(buff.get(), 1, len, fp);
		if (bread <= 0)
			break;

		for (int i = 0; i < bread; i++)
		{
			uint64_t *pmark = (uint64_t*)(buff.get() + i);
			if (*pmark == marker) {
				memcpy(&es, pmark, sizeof(PackedBuffer));
				return true;
			}
		}

	}

	return false;
}


bool write_pb(FILE *fp, PackedBuffer &pb)
{
	// Same bug as with previous function
	const int len = 1024 * 1024;
	unique_ptr<char> buff(new char[len]);

	fseek(fp, 0, SEEK_SET);

	unsigned wpos = 0;
	while (1)
	{
		int bread = fread(buff.get(), 1, len, fp);
		if (bread <= 0)
			break;

		for (int i = 0; i < bread; i++)
		{
			uint64_t *pmark = (uint64_t*)(buff.get() + i);
			if (*pmark == pb.marker)
			{
				fseek(fp, wpos + i, SEEK_SET);
				fwrite(&pb, sizeof(PackedBuffer), 1, fp);
				return true;
			}
		}
		wpos += len;
	}

	return false;
}


void set_value(FILE *fp, uint64_t marker, const char *value)
{
	PackedBuffer pb;

	if (read_pb(fp, marker, pb) == false)
	{
		puts("Cannot find specified marker\n");
		return;
	}

	strcpy(pb.buffer, value);

	if(write_pb(fp, pb)==false)
	{
		puts("Cannot find specified marker while writing\n");
		return;
	}
}



int main()
{
	FILE *fp = fopen("../Debug/Target.exe", "r+b");
	if (fp == NULL)
	{
		perror("Cannot open specified file\n");
		return 1;
	}


	set_value(fp, MARK_TEST, "Modified test value");
	set_value(fp, MARK_ANOTHER, "ANTHER VALUE MODIFIED");

	fclose(fp);

	puts("Press ENTER to close");
	getchar();

    return 0;
}

