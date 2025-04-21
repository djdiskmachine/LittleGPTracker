
#ifndef _VITA_FILESYSTEM_H_ 
#define _VITA_FILESYSTEM_H_ 

#include "System/FileSystem/FileSystem.h"
#include <psp2/io/stat.h>
#include <psp2/io/dirent.h>
#include <psp2/io/fcntl.h>

#define WRITE_BUFFER_SIZE 1024

class VITAFile: public I_File {
public:
	VITAFile(SceUID) ;
	virtual ~VITAFile() ;
	virtual int Read(void *ptr, int size, int nmemb) ;
	virtual int Write(const void *ptr, int size, int nmemb) ;
	virtual void Printf(const char *format,...);
	virtual void Seek(long offset,int whence) ;
	virtual long Tell() ;
	virtual void Close() ;
protected:
	void flush() ;
private:
	SceUID file_ ;
	unsigned char writeBuffer_[WRITE_BUFFER_SIZE] ;
	int writeBufferPos_ ;
	
} ;

class VITADir: public I_Dir {
public:
    VITADir(const char *path) ;
	virtual ~VITADir() {} ;
    virtual void GetContent(char *mask) ;
} ;

class VITAFileSystem: public FileSystem {
public:
	virtual I_File *Open(const char *path,char *mode);
	virtual I_Dir *Open(const char *path) ;
	virtual FileType GetFileType(const char *path) ;
	virtual Result MakeDir(const char *path) ;
	virtual void Delete(const char *path) ;
} ;
#endif
