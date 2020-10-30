#!/usr/bin/env python3
# -*- coding: utf-8 -*-
__author__ = "MPZinke"

###########################################################################
#
#	created by: MPZinke
#	on ..
#
#	DESCRIPTION:
#	BUGS:		- Does not prevent overlap.
#				- Does not prevent blocking off section of field.
#	FUTURE:
#
###########################################################################

from random import randint;


ROWS = 7;  # row count (x-value)
COLUMNS = 17;  # column count (y-value)

SQUARE_SIZE = 0.305;

ADD_BORDER = True;  # add obstacles along border
NUMBER_OF_OBSTACLES = 69;


# Optional function to add obstacle border based on ADD_BORDER global bool.
# Takes 2D list of field.
# Goes through outer points and sets value to the SQUARE_SIZE factor.
# Returns number of obstacles used to do this process.
def create_border(field):
	for x in range(COLUMNS): field[0][x] = field[ROWS-1][x] = SQUARE_SIZE;
	for x in range(1, ROWS-1): field[x][0] = field[x][COLUMNS-1] = SQUARE_SIZE;
	return COLUMNS * 2 + (ROWS - 2) * 2;


# Create a field for the give dimensions.
# Returns a 2D array of Rows x Columns.
def create_field():
	return [[0 for x in range(COLUMNS)] for y in range(ROWS)];


# Creates & adds random obstacles to the board.
# Takes the field array, the maximum number of obstacles allowed to create.
# Picks an orientation (0: horizontal, 1: vertical), finds an obstacle length that will work.
# Picks a random position. Adds SQUARE_SIZE to parts of obstacles.
# Returns length (number of obstacles used).
def create_obstacles(field, remaining_obstacles):
	orient = randint(0, 1);  # 0: horizontal, 1: vertical
	length = randint(1, min(remaining_obstacles, [COLUMNS, ROWS][orient] - 3));  # - 3 to make it passable
	pos = [randint(1, ROWS - (orient * length) - 2), randint(1, COLUMNS - ((not orient) * length) - 2)];

	for x in range(length): field[(orient * x) + pos[0]][((not orient) * x) + pos[1]] = SQUARE_SIZE;
	return length;


# Prints the field, optionally with col/row numbers.
# Takes the 2D list of the field.
# Prints square by square (with X meaning filled, - meaning empty).
def print_field(field, with_markers=True):
	if(with_markers): print(end=" ");
	for y in range(COLUMNS): print("%3d" % (y), end="");
	if(with_markers): print();

	for x in range(ROWS):
		if(with_markers): print("%-2d" % (x), end="");
		for y in range(COLUMNS):
			print(' - ' if not field[x][y] else ' X ', end='');
		print();
	print();


# Prints C_code array.
# Takes the 2D list field.
# Prints the declaration.  Then the instantiation.
# Formats it to be pretty :)
def print_C_code(field):
	print("double obstacle[{}][2] =".format(NUMBER_OF_OBSTACLES));
	for x in range(ROWS):
		for y in range(COLUMNS):
			if(field[x][y]): 
				if(not x and not y): print_string = "{%.1f, %.1f}, ";
				elif(not x): print_string = "{%.1f, %.4f}, ";
				elif(not y): print_string = "{%.4f, %.1f}, ";
				else: print_string = "{%.4f, %.4f}, ";
				print(print_string % (x * SQUARE_SIZE, y * SQUARE_SIZE), end="");
		print()
	print("};");


def main():
	field = create_field();
	if(ADD_BORDER): remaining_obstacles = NUMBER_OF_OBSTACLES - create_border(field);
	else: remaining_obstacles = NUMBER_OF_OBSTACLES;

	while remaining_obstacles: remaining_obstacles -= create_obstacles(field, remaining_obstacles);
	print_field(field);
	print_C_code(field);


if __name__ == "__main__":
	main()
