#ifndef _DIRECTORY_H_
#define _DIRECTORY_H_

#define MAX_NAME_LEN	32
#define MAX_LABEL_LEN	16


struct dir_entry_s {
	char	name[ MAX_NAME_LEN ];
	int		flags;
	void	*contents;
	struct dir_entry_s *parent;
	struct dir_entry_s *next;
};

#endif /* _DIRECTORY_H_ */
