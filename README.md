C-JSON by JoshOY
============================
##What's this

A light weight JSON parser, which is the course project of C programming, Tongji Univ.<br>
The library provides some interfaces for parsing JSON.<br>
I made some changes and it can format JSON with standard formation.<br>

for example, here's a JSON with terrible format:<br>
'''
[1,    2, {
           "name" :    "JoshOY",
      major   :  "SE"
}]
'''

And it can format into

'''
[
	1,
	2,
	{
		"name": "JoshOY",
		"major": "SE"
	}
]
'''

##How to get it
'''
$ git clone https://github.com/JoshOY/C-JSON-joshoy.git
$ cd C-JSON-joshoy
$ make
'''

And that's done.

##How to use it
Here are the commands:

-h    Show the info.
-p    Parse a JSON string in argument and print it in console with no spaces.
-r    Read JSON from a file and print it in console with no spaces.
-w    Write the buffer JSON into a file with format. The buffer comes from -p or -r and if you
      there's no parsed JSON it will create a 'null' JSON instead.
      For example, './C-JSON -r ./input.json -w ./output.json'

Type ./C-JSON -h for some more information.

