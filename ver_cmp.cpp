#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rpm/rpmlib.h>
#include <mysql/mysql.h>
#include <apt-pkg/init.h>
#include <apt-pkg/version.h>

extern "C" { 
	my_bool ver_cmp_init(UDF_INIT *, UDF_ARGS *, char *);
	void ver_cmp_deinit(UDF_INIT *);
	long long ver_cmp(UDF_INIT *, UDF_ARGS *, char *, char *);
}

int len1;
int len2;
char *version1;
char *version2;
extern pkgSystem *_system;
extern Configuration *_config;

my_bool ver_cmp_init(UDF_INIT *initid, UDF_ARGS *args, char *msg)
{
	/* apt-pkg init start */
	if((not pkgInitConfig(*_config)) or (not pkgInitSystem(*_config, _system)))
	{
		strcpy(msg, "ver_cmp_init(): failed to initSystem!");
		return -1;
	}
	/* apt-pkg init end */

	return 0;
}

void ver_cmp_deinit(UDF_INIT *initid __attribute__((unused)))
{
	free(version1);
	free(version2);
}


long long ver_cmp(UDF_INIT *initid, UDF_ARGS *args, char *is_null, char *error)
{
	// _system->VS->DoCmpVersion(v1, v1+len1, v2, v2+len2)
	// the end pointer passed to apt_pkg: DoCmpVersion seems not working, it still sees the version in wrong length.
	// So we have to copy it to another block of memory and set '\0' at the end.
			
	len1 = args->lengths[1];
	len2 = args->lengths[2];
	version1 = (char *)malloc(len1+1);
	version2 = (char *)malloc(len2+2);
	strncpy(version1, args->args[1], len1);
	strncpy(version2, args->args[2], len2);
	memset(version1+len1, 0, 1);
	memset(version2+len2, 0, 1);

	if((strcmp(args->args[0], "rpm") == 0))
		return rpmvercmp(version1, version2);
	else
		return _system->VS->DoCmpVersion(version1, version1+len1, version2, version2+len2);
}

/*
int main()
{
	if((not pkgInitConfig(*_config)) or (not pkgInitSystem(*_config, _system)))
	{
		printf("ver_cmp_init(): failed to initSystem!");
		return -1;
	}
	char v1[] = {"4.4-5"};
	char v2[] = {"2:4.4-1"};
	printf("%d\n", _system->VS->DoCmpVersion(v1, v1+strlen(v1), v2, v2+strlen(v2)));


	char v3[] = {"4.4-5"};
	char v4[] = {"4.4-6"};
	printf("%d\n", rpmvercmp(v3, v4));
	return 0;
}*/
