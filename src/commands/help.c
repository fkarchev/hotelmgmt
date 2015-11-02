/* help.c */
/* Date  : 02 November 2015
 * Author: Ankit Pati
 */

#include "../defs/functions.h"
#include "../defs/structures.h"

void help_ls()
{
    puts(
        "Available commands:\n"
        "help [<command name>]\n"
        "prompt [on | off]\n"
        "init [<hotel name>]\n"
        "drop [<hotel name>]\n"
        "create-service [<service name>]\n"
        "list [hotels | services | guests]\n"
        "switch [<hotel name>]\n"
        "back\n"
        "book [<guest name>]\n"
        "service [<service name>]\n"
        "bill [<guest name>]\n"
        "checkout [<guest name>]"
    );
}

void help_help()
{
    puts(
        "help [<command name>]\n"
        "Displays usage information for the specified command. Displays all\n"
        "available commands if command name is not provided as parameter."
    );
}

void help_prompt()
{
    puts(
        "prompt [on | off]\n"
        "Sets the display of the prompt to on or off if specified. If called\n"
        "without parameters, displays the prompt once."
    );
}

void help_init()
{
    puts(
        "init [<hotel name>]\n"
        "Creates a binary file with the .hms (Hotel Management System)\n"
        "extension. Prompts for hotel name if not provided as parameter, and\n"
        "for number of rooms."
    );
}

void help_drop()
{
    puts(
        "drop [<hotel name>]\n"
        "Deletes the disk file associated with the mentioned hotel. Prompts\n"
        "for hotel name if not provided as parameter."
    );
}

void help_create_service()
{
    puts(
        "create-service [<service name>]\n"
        "Appends to services.srv binary file the name and cost of services\n"
        "available. Prompts for name is not provided as parameter, and for\n"
        "cost."
    );
}

void help_list()
{
    puts(
        "list [hotels | services | guests]\n"
        "Lists hotels, services, or guests, as directed. Prompts for input if\n"
        "no parameter is provided."
    );
}

void help_switch()
{
    puts(
        "switch [<hotel name>]\n"
        "Switches to the mentioned hotel, so that further operations may be\n"
        "relative to it. Prompts for hotel name if not provided as parameter.\n"
        "Subsequent commands display the '@' character, followed by the hotel\n"
        "name as prompt."
    );
}

void help_back()
{
    puts(
        "back\n"
        "Takes no parameter. Switches the guest out of the currently selected\n"
        "hotel, and into the main shell. The '@' hotel name prompt is cleared."
    );
}

void help_book()
{
    puts(
        "book [<guest name>]\n"
        "Adds an entry into the associated .hms file for the guest, if rooms\n"
        "are available. Prompts for guest name if not provided as input."
    );
}

void help_service()
{
    puts(
        "service [<service name>]\n"
        "Records a service against the bill of a guest. Prompts for service\n"
        "name if not provided as parameter, and for guest name."
    );
}

void help_bill()
{
    puts(
        "bill [<guest name>]\n"
        "Prepares and displays the bill for the guest. Prompts for guest name\n"
        "if not provided as parameter."
    );
}

void help_checkout()
{
    puts(
        "checkout [<guest name>]\n"
        "Prepares and displays the bill for the guest, and removes entry from\n"
        "associated .hms file. Prompts for guest name if not provided as\n"
        "parameter."
    );
}

void help(char *command)
{
         if(!command) help_ls();
    else if(!strcmp(command, "help")) help_help();
    else if(!strcmp(command, "prompt")) help_prompt();
    else if(!strcmp(command, "init")) help_init();
    else if(!strcmp(command, "drop")) help_drop();
    else if(!strcmp(command, "create-service")) help_create_service();
    else if(!strcmp(command, "list")) help_list();
    else if(!strcmp(command, "switch")) help_switch();
    else if(!strcmp(command, "back")) help_back();
    else if(!strcmp(command, "book")) help_book();
    else if(!strcmp(command, "service")) help_service();
    else if(!strcmp(command, "bill")) help_bill();
    else if(!strcmp(command, "checkout")) help_checkout();
    else error_print("Unknown command!");
}
/* end of help.c */
