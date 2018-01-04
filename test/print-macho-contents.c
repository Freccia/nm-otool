#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <mach-o/loader.h>

void printLoadCommandsInDetail(unsigned char* buf)
{
    struct load_command *cmd = (struct load_command*) buf;
    
    switch(cmd->cmd) {
        case LC_CODE_SIGNATURE:
        {
            struct linkedit_data_command *lc_code_sig = (struct linkedit_data_command*) buf;
            printf("\n%s\n", "LC_CODE_SIGNATURE");
            printf("\tcmd\t\t= %d\n", lc_code_sig->cmd);
            printf("\tcmdsize\t= %d\n", lc_code_sig->cmdsize);
            printf("\tdataoffset = %d\n", lc_code_sig->dataoff);   
            printf("\tdatasize = %d\n", lc_code_sig->datasize); 
            break;
        }
        case LC_SEGMENT:
        {
            struct segment_command *lc_seg = (struct segment_command*) buf;
            printf("\n%s\n", "LC_SEGMENT");
            printf("\tsegname\t\t= %s\n", lc_seg->segname);
            printf("\tcmd\t\t= %d\n", lc_seg->cmd);
            printf("\tcmdsize\t= %d\n", lc_seg->cmdsize);
            printf("\tsegnament= %s\n", lc_seg->segname);
            printf("\tdataoffset = %d\n", lc_seg->fileoff);  
            printf("\tdatasize = %d\n", lc_seg->filesize); 
            
            break;
        }
        case LC_SYMTAB:
        {
            struct symtab_command *lc_symtab = (struct symtab_command*) buf;
            printf("\n%s\n", "LC_SYMTAB");
            printf("\tcommandsize\t\t= %d\n", lc_symtab->cmdsize);
            printf("\tsymbol table offset\t\t= %d\n", lc_symtab->symoff);
            printf("\tnumber of symbols\t= %d\n", lc_symtab->nsyms);
            printf("\tstring table offset= %d\n", lc_symtab->stroff);
            printf("\tstring table size = %d\n", lc_symtab->strsize);
            break;
        }
        case LC_LOAD_DYLINKER:
        {
            struct dylinker_command *lc_dylinker = (struct dylinker_command*) buf;
            printf("\n%s\n", "LC_LOAD_DYLINKER");
            printf("\tcommandsize\t\t= %d\n", lc_dylinker->cmdsize);
            printf("\tstr offset\t= %d\n", lc_dylinker->name.offset);
            break;
        }
        case LC_UUID:
        {
            struct uuid_command *lc_uuid_command = (struct uuid_command*) buf;
            printf("\n%s\n", "LC_UUID");
            printf("\tcommandsize\t\t= %d\n", lc_uuid_command->cmdsize);
            printf("\tstr offset\t= %s\n", lc_uuid_command->uuid);
            break;
        }
        case LC_VERSION_MIN_IPHONEOS:
        {
            struct version_min_command *lc_version_min_command = (struct version_min_command*) buf;
            printf("\n%s\n", "LC_VERSION_MIN_IPHONEOS");
            printf("\tcommandsize\t\t= %d\n", lc_version_min_command->cmdsize);
            printf("\tstr version\t= %u\n", lc_version_min_command->version);
            printf("\tstr Reserved\t= %u\n", lc_version_min_command->reserved);
            break;
        }
        //TODO
        //print all other load commands
    
    }
    
    return;
}



void printLoadCommands(FILE *fp,  struct mach_header *mach_header_ptr) {
    long cmdsize = sizeof(struct load_command);
    struct load_command *load_command_ptr;
    unsigned char* buf = 0;
    int i = 0;
    for (i=0; i < mach_header_ptr->ncmds; ++i) {
        long tmpsize = 0;
        
        load_command_ptr = calloc(1, cmdsize);
        
        if (!load_command_ptr) {
            printf("Memory Error: Couldn't allocate <%zu>", cmdsize);
            exit(1);
        }
        fread(load_command_ptr, cmdsize, 1, fp);
        tmpsize = load_command_ptr->cmdsize;        
        buf = malloc(tmpsize);
        if (!buf) {
            printf("Memory Error: Couldn't allocate <%zu>", tmpsize);
            exit(1);
        }
        bzero(buf, tmpsize);
        memcpy(buf, load_command_ptr, sizeof(cmdsize));
        fread(buf+cmdsize, tmpsize - cmdsize, 1,fp);
        printLoadCommandsInDetail(buf);
         
        free(buf); buf = 0;
        free(load_command_ptr); load_command_ptr = 0;
    }
}

void printMacOContents(FILE * fp)   {
     struct mach_header *mach_header_ptr;
     long hdrsize = 0;
     hdrsize = sizeof(struct mach_header);
     mach_header_ptr = calloc(1,hdrsize);
     fread(mach_header_ptr, hdrsize, 1, fp);
     printLoadCommands(fp,mach_header_ptr);
}

int main(int argc, char **argv) {
     FILE *infile    = fopen(argv[1], "rb");
     printMacOContents(infile);
}
