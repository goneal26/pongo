# prototype for interpreter in python

import sys

had_error = False

def main():
	arg_count = len(sys.argv)

	if (arg_count > 2):
		print("Usage: pypongo [script]")
		# exit code 64
	elif (arg_count == 2):
		# if argv[1] is '-h' print help info
		print(f"Running file {sys.argv[1]}...\n")
		run_file(sys.argv[1])
	else:
		print("Entering prompt mode...\n")
		run_prompt()

def error(line, message):
	report(line, "", message)

def report(line, where, message):
	print(f"[line {line}] Error{where}: {message}")
	had_error = True

if __name__ == '__main__':
	main()