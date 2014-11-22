/*

  parser.c
  bho1 - init 27.8.2012 

  Compile with  `sdl-config --cflags --libs`
  

  
*/
#include <malloc.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>


#include "cpu.h"
#include "flags.h"
#include "alu.h"
#include "decode.h"
#include "memory.h"
#include "cpu-util.h"
#include "register.h"
#include "src-ripper.h"
#include "printers.h"

#include "SDL.h"

#include "bitmap.h"

extern int videobase;

/* A static variable for holding the line. */
static char *line_read = (char *)NULL;

/* Read a string, and return a pointer to it.
   Returns NULL on EOF. */
char *
rl_gets ()
{
  /* If the buffer has already been allocated,
     return the memory to the free pool. */
  if (line_read)
    {
      free (line_read);
      line_read = (char *)NULL;
    }

  /* Get a line from the user. */
  line_read = readline("");

  /* If the line has any text in it,
     save it on the history. */
  if (line_read && *line_read)
    add_history(line_read);

  return (line_read);
}


char * read_line(){
  return rl_gets ();
}

void repl(){
  int adr, content,arg1,arg2;
  char buffer_reg[8+1];

  char cmd[3];
  char reg[3];
  
  char *  string;

  void (*mue_code_ptr)();
  
  //  char *cmd, *reg;
  

  init_src();
  printf("src at %4x is :\n", 0x0600);
  src_read("/media/c5bcf67f-e9eb-4e12-be75-d8b6e09e27ba/olivier/hti/ginf/cpu-emu/cpu-emu/sipiu-codebase/demoscene/demoscene.dis");
  src_print(0x0600);

  string = read_line();

  int breaklabel;
  while(string && strcmp(string,"exit")!=0){

    sscanf(string, " %s %s ", &cmd, &reg);

    
    if(strcmp(cmd,"ls")==0){
      print_register_exec();
      //printf("reaqdline : %s :: %s %s\n", string, cmd, reg);
    }
    //    if(strcmp(cmd,"g")==0){
    //  print_register_exec();
      //printf("reaqdline : %s :: %s %s\n", string, cmd, reg);
    //}
    if(strcmp(cmd,"quit")==0){
      printf("quit repl and cpu ..\n");
      exit(0);
    }
    
    /*
      register setting:
      <reg-name> <value>
      <reg-name> ::= idx idy idz acc, pc abrl abrh ir
      
    */
    if(strcmp(cmd,"until")==0){
      //        if(strcmp(cmd,"break")==0){
      sscanf(string, " %s %4x ", &cmd, &adr);
      breaklabel = adr;
      while(breaklabel!=pc2int()){
	fetch();
	mue_code_ptr = decode();
	execute(mue_code_ptr);

	print_registers();//_exec();
      }
    }else
    if(strcmp(cmd,"abrh")==0){
      sscanf(string, " %s %2x ", &cmd, &content);
      number2register(content, buffer_reg);
      cp_register(buffer_reg, abrh);
    }else
      if(strcmp(cmd,"abrl")==0){
	sscanf(string, " %s %2x ", &cmd, &content);
	number2register(content, buffer_reg);
	cp_register(buffer_reg, abrl);
      }else
	if(strcmp(cmd,"a")==0){
	  sscanf(string, " %s %2x ", &cmd, &content);
	  number2register(content, buffer_reg);
	  cp_register(buffer_reg, acc);
	}else
	  if(strcmp(cmd,"ir")==0){
	    sscanf(string, " %s %2x ", &cmd, &content);
	    number2register(content, buffer_reg);
	    cp_register(buffer_reg, ir);
	  }else
	    if(strcmp(cmd,"pcl")==0){
	    sscanf(string, " %s %2x ", &cmd, &content);
	    number2register(content, buffer_reg);
	    cp_register(buffer_reg, pcl);
	  }else
	    if(strcmp(cmd,"pch")==0){
	    sscanf(string, " %s %2x ", &cmd, &content);
	    number2register(content, buffer_reg);
	    cp_register(buffer_reg, pch);
	  }else
	    

    
	    /*
	      control
	      <reg-name> <value>
	      <control> ::= next step
	      
	    */  
	    if(strcmp(cmd,"next")==0){
	      fetch();
	      mue_code_ptr = decode();
	      execute(mue_code_ptr);
	    }else
	      
	    if(strcmp(cmd,"step")==0){
	      fetch();
	      mue_code_ptr = decode();
	      execute(mue_code_ptr);
	      print_registers();//_exec();
	      //	      src_print(pc2int());
	    }else
	      /*
		memory
		memarr <adr1> <adr2>
		mem <adr> <value>
		mem <adr>
      
      
	      */
	      if(strcmp(cmd,"mem")==0){
		printf("print_memory(memory[arg1]\n");
		arg1=0;
		sscanf(string, " %s %4x ", &cmd, &arg1);
		print_memory_page2(arg1);
		/* if(arg2){ */
		/*   printf("ARG2 : %d print_memory(memory[arg1]\n",arg2); */
		/*   number2register(arg2, buffer_reg); */
		/*   cp_byte(memory[arg1], buffer_reg); */
		/*   // memory[arg1] = arg2; */
		/* }else { */
		/*   printf("YEZZ:print_memory(memory[arg1]\n"); */
		/*   print_memory(memory[arg1]); */
		/* } */
	      }else{
		
		
		sscanf(string, " %4x %2x ", &adr, &content);
		if(adr>MEMORY_SIZE){
		  printf("parse memory: Illegal Address %i\nSkipping line ..\n", adr);
		} else {
		  //	  printf("%i: %i->%s\n", i++, adr, buffer_reg);
		  number2register(content, buffer_reg);
		  cp_cell2memory(buffer_reg, adr);
		}
	      }
    int f;
    f = cycles%100;
    if(f)
      draw_bitmap_memory(videobase);        
    printf("reaqdline : %s :: %i %i\n", string, adr, content);
    string = read_line();
    
    /* sscanf(string, "%2x %2x %2x\n", &x,&y,&z); */
    /* printf("result is : %s :: %i \n>>>", string, x); */
    /* scanf("%s\n", string); */
  }
}

