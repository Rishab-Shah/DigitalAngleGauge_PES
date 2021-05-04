/******************************************************************************
* @file: commandprocessor.c
*
* @brief: This files consists of the various commands received from the user
*
* @author: Rishab Shah
* @date:  03-May-2021
* @references: Howdy lecture notes
*******************************************************************************/
/*******************************************************************************
Header files
*******************************************************************************/
#include "commandprocessor.h"
/*******************************************************************************
Macros
*******************************************************************************/
#define NO_OF_COMMANDS 					(sizeof(commands)/sizeof(commands[0]))
/*******************************************************************************
Function declarations
*******************************************************************************/
static void author_command_handler(const char *datareceived);
static void help_command_handler(const char *datareceived);
static void unknown_command_handler(const char *datareceived);
static void insight_command_handler(const char *datareceived);
/*******************************************************************************
GLobal variables, structures
*******************************************************************************/
int reference_angle = 0;

typedef void (*command_handler_t)(const char* datareceived);

typedef struct
{
	const char* commandname;
	command_handler_t handlername;
	const char* commandparameters;
}command_table_t;

static const command_table_t commands[] =
{
	{"author"  ,author_command_handler     ,"Print the author\r\n	 <no parameters>"									},
	{"insight" ,insight_command_handler    ,"Print the relative reference angle\r\n	 <no parameters>"					    },
	{"help"	   ,help_command_handler	   ,"Print this help message\r\n	 <no parameters>"							},
};


/*******************************************************************************
Function Definitions
*******************************************************************************/
/*******************************************************************************
* @Function Name: lexical_analysis
* @Description: performs the lexical analysis. multiple spaces are removed.
* only one space is kept post a character string and is passed to the next
* function to execute commands
* @input param : the string of commands
* @return: None
*******************************************************************************/
void lexical_analysis(uint8_t *input)
{
	uint8_t *p = input;
	uint8_t *end;
	uint8_t tempcounter = 0;

	bool spacedetected = true;

	char tempbuf[200] = {0};
	char *tempptr = tempbuf;
	char commandreceived[25];
	memset(commandreceived,'\0',sizeof(commandreceived));

	/* find end of string */
	for(end = input;*end != '\0';end++)
	{
		tempcounter++;
	}

	for(p = input; p < end;p++)
	{
		if((*p == ' '))
		{
			if(!spacedetected)
			{
				/* inserting space only once after the
				 * end of the string.
				 * Extra spaces are removed.
				 * Will be used further in sscanf to tokenize */
				*tempptr++ = ' ';
				/* Do nothing as space is to be ignored */
			}

			spacedetected = true;
		}
		else
		{
			/* Enter when a character is detected */
			*tempptr++ = *p;

			spacedetected = false;
		}
	}

	*tempptr++ = '\0';

	tempptr = &tempbuf[0];

	/* extract the first string  for command detection */
	sscanf(&tempbuf[0],"%s",commandreceived);

	for(uint8_t currentcommand = 0; currentcommand < NO_OF_COMMANDS;currentcommand++)
	{
		if(strcasecmp(commandreceived,commands[currentcommand].commandname) == 0)
		{
			commands[currentcommand].handlername(tempbuf);
			return;
		}
	}

	/* if the command is not recognized
	 * call the below handler */
	unknown_command_handler(tempbuf);
	return;
}

/*******************************************************************************
* @Function Name: author_command_handler
* @Description: a function pointer. executes the author command. displays the
* author's name
* @input param : command received
* @return: None
*******************************************************************************/
static void author_command_handler(const char *datareceived)
{
	printf("\r\nRishab Shah\r\n");
}

/*******************************************************************************
* @Function Name: help_command_handler
* @Description: a function pointer. executes the help command dispalying all the
* commands present
* @input param : command received
* @return: None
*******************************************************************************/
static void help_command_handler(const char *datareceived)
{
	printf("\r\nDigital Angle Gauge Serial Command Processor\r\nFollowing commands are available \r\n");

	for(uint8_t cmdlen = 0; cmdlen < NO_OF_COMMANDS; cmdlen++)
	{
		printf("%s\r\n\t %s\r\n",commands[cmdlen].commandname,
							commands[cmdlen].commandparameters);
	}
}

/*******************************************************************************
* @Function Name: unknown_command_handler
* @Description: a function pointer. executes the unknown command
* @input param : command received
* @return: None
*******************************************************************************/
static void unknown_command_handler(const char *datareceived)
{
	if(strcasecmp(datareceived,"\r\n") == 0)
	{
		/* handles the new line printing when enter key is pressed */
		printf("\r\n");
	}
	else
	{
		printf("\r\nUnknown command: %s",datareceived);
	}
}

/*******************************************************************************
* @Function Name: insight_command_handler
* @Description: gives the value of the reference angle set currently
* @input param : command received
* @return: None
*******************************************************************************/
static void insight_command_handler(const char *datareceived)
{
	printf("\r\nReference value set is %d\r\n",reference_angle);
}

/*******************************************************************************
* @Function Name: set_reference_angle
* @Description: sets the reference value
* @input param : the mma roll value received
* @return: None
*******************************************************************************/
void set_reference_angle(int ref_rcvd )
{
	reference_angle = ref_rcvd;
}

/* EOF */
