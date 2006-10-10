/*
 * $Id$
 * Copyright (c) 2006, IRIT-UPS.
 *
 * src/system_ProcessBuilder.cpp -- ProcessBuilder class implementation.
 */

#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <elm/system/ProcessBuilder.h>
#include <elm/system/SystemException.h>

extern char **environ;
 
namespace elm { namespace system {

/**
 * @class ProcessBuilder
 * This class is used to build a new process by launching a command line.
 */


/**
 * Construct a process builder.
 * @param command	Command to use.
 */
ProcessBuilder::ProcessBuilder(CString command)
:	in(&io::stdin),
	out(&io::stdout),
	err(&io::stderr)
{
	args.add(command);
}


/**
 * Add an argument to the command line.
 * @param argument	Argument to add.
 */	
void ProcessBuilder::addArgument(CString argument) {
	args.add(argument);
}


/**
 * Set the input stream of the built process.
 * @param _in	New process input.
 */
void ProcessBuilder::setInput(SystemInStream *_in) {
	in = _in;
}


/**
 * Set the output stream of the built process.
 * @param _out	New process output.
 */
void ProcessBuilder::setOutput(SystemOutStream *_out) {
	out = _out;
}


/**
 * Set the error stream of the built process.
 * @param _out	New process error.
 */
void ProcessBuilder::setError(SystemOutStream *_err) {
	err = _err;
}


/**
 * Run the built process.
 * @return	The built process.
 * @throws SystemException	Thrown if there is an error during the build.
 */
Process *ProcessBuilder::run(void) {
	int old_in = -1, old_out = -1, old_err = -1;
	int error = 0;
	Process *process = 0;
	
	// Prepare the streams
	if(in->fd() != io::stdin.fd()) {
		old_in = dup(0);
		if(old_in < 0)
			error = errno;
		else
			dup2(in->fd(), 0);
	}
	if(!errno && out->fd() != io::stdout.fd()) {
		old_out = dup(1);
		if(old_out < 0)
			error = errno;
		else
			dup2(out->fd(), 1);
	}
	if(!errno && err->fd() != io::stderr.fd()) {
		old_err = dup(2);
		if(old_err < 0)
			error = errno;
		else
			dup2(err->fd(), 2);
	}
	
	// Create the process
	if(!error) {
		int pid = fork();
		if(pid < 0)
			error = errno;
		else if(pid != 0)
			process = new Process(pid);
		else {
			
			// Build arguments
			char *tab[args.length() + 1];
			for(int i = 0; i < args.length(); i++)
				tab[i] = (char *)&args[i];
			tab[args.length()] = 0;
			
			// Launch the command
			execvp(tab[0], tab);
			exit(1);
		}
	}
	
	
	// Reset the streams
	if(old_in >= 0) {
		close(0);
		dup2(old_in, 0);
		close(old_in);
	}
	if(old_out >= 0) {
		close(1);
		dup2(old_out, 1);
		close(old_out);
	}
	if(old_err >= 0) {
		close(2);
		dup2(old_err, 2);
		close(old_err);
	}
	
	// Return the result
	if(error)
		throw new SystemException(error, "process building");
	else
		return process;
}

} } // elm::system