#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <mach-o/loader.h>

static int
nm(char const * file) {
    printf("opening %s\n", file);
	int fd = open(file, O_RDONLY);
	struct stat s;
	void * data;
	struct load_command * load_command;
	struct mach_header * header;

	if (fd == -1)
		return (EXIT_FAILURE);
	else if (fstat(fd, &s) == -1 || s.st_size == 0)
		return (EXIT_FAILURE);
	else if ((data = mmap(NULL, s.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
		return (EXIT_FAILURE);

	header = data;
	printf("macho = %i\n", header->magic == MH_MAGIC || header->magic == MH_CIGAM
	|| header->magic == MH_MAGIC_64 || header->magic == MH_CIGAM_64);
	load_command = (void*)(header + 1);
	for (uint32_t i = 0; i < header->ncmds; ++i) {
		printf("cmd size = %i\n", load_command->cmdsize);
		load_command = (void*)load_command + load_command->cmdsize;
	}
	close(fd);
	munmap(data, s.st_size);
	return (EXIT_SUCCESS);
}

int main(int ac, char ** av) {
    if (ac > 2) {
       fprintf(stderr, "usage: ft_nm [file]\n");
		return (EXIT_FAILURE);
    }
    return (nm(ac > 1 ? av[1]: "a.out"));
}