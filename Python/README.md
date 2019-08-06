This was a little project I made on a whim when I became interested in password hashing during my Senior year of high school. All of the code is mine. 
Run the script with ./crackpassword.py - note that you may need to modify the first line based on your operating system, as described in the file
The string comparison function is merely a dependency. The script will give the user a few options:
    Making will, given a few parameters, return a hash in the specified form.
    Solving will, given a few parameters, eventually (with stress on eventually) find the original plaintext password.
    
When entering text, one significant word will suffice for a line ("Solve for Password" can be simplified to "solve").
Unfortunately, python does not actually take advantage of multiple processor units, so making threads doesn't increase efficiency, but gets to high-character words sooner.
