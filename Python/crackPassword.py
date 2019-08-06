#!/usr/local/bin/python3

#make /usr/local/bin/python3 for mac, /usr/bin/python3 for linux

#
# Written fully by Andrew Wells
# dependant on stringComparison.py
# written for fun in late 2015, in a spurt of cryptographic interest
#
#

import hashlib
import datetime
import threading
import random
import stringComparison as compare

c = ['a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9']
class cycleThread(threading.Thread):
    def __init__(self, ThreadID, minLength, maxLength, hashValue, salt, c, hashType, digestType):
        threading.Thread.__init__(self) # both these references to self are necessary
        self.ThreadID = ThreadID        # notice that self isn't passed in when I actually call this
        self.minLength = minLength
        self.maxLength = maxLength
        self.hashValue = hashValue
        self.salt = salt
        self.c = c
        self.hashType = hashType
        self.digestType = digestType
        self.threads = []
        self.kill = False
    def run(self):
        print("Thread " + str(self.ThreadID) + " started, testing " + str(self.minLength) + " to " + str(self.maxLength-1) + " characters.")
        success = self.getPasswordFor(self.hashValue, self.salt, self.minLength, self.maxLength, self.hashType, self.digestType)
        if(success):
            print("Thread " + str(self.ThreadID) + " ended in success!")
            self.killAll()
        else:
            print("Thread " + str(self.ThreadID) + " ended in failure.")
    def killAll(self):
        for x in range(len(self.threads)):
            self.threads[x].kill = True
            print("Thread " + str(self.threads[x].ThreadID) + " killed by " + str(self.ThreadID))
    def getPasswordFor(self, hashValue, salt, minLengthInclusive, maxLengthInclusive, hashType, digestType):
        iCount = 0
        bRun = True
        bCracked = False
        index = []
        for x in range(minLengthInclusive):
            index.append(0)
        archiveTime = datetime.datetime.now()
        displayEvery = 2000000                                      #modify per desire
        while(bRun and (not bCracked)):
            string = ""
            for x in range(len(index)):
                string += c[index[x]]

		#begin testing types and strings

            test = makeHash(hashType, digestType, string, salt)

		#end testing
            iCount += 1
            if(test == hashValue):
                bCracked = True
            else:
                iDisplay = iCount % displayEvery
                if(iDisplay == 0):
                    now = datetime.datetime.now()
                    seconds = ((now.minute - archiveTime.minute)*60)+(now.second - archiveTime.second)
                    pps = 0 # passwords per second
                    if(seconds != 0):
                        pps = displayEvery / seconds
                        pps = int(pps)
                    print("Thread " + str(self.ThreadID) + "\n\tPasswords tested: " + str(iCount) + "\n\t" + str(pps) + " passwords/second\n\tCurrently testing: " + string + " (" + str(len(string)) + " characters)")
                    archiveTime = now
                index[len(index)-1] += 1 #begin next string creation
                if(index[len(index)-1] == len(c)):
                    for z in range(len(index)-1):
                        d = z + 1
                        if(index[len(index)-d] == len(c)):
                            index[len(index)-d] = 0
                            index[len(index)-d-1] += 1
                if(index[0] == len(c)):
                    index[0] = 0
                    index.append(0)
                    if(len(index) > maxLengthInclusive):
                        bRun = False
                if(self.kill == True):
                    bRun = False
        if(bCracked):
            string = ""
            for x in range(len(index)):
                    string += c[index[x]]
            print("Password is " + string + ", " + str(iCount) + " attempts")
            return True
        else:
            return False
def main():
    menu = ["Make Hash", "Solve for Password", "Estimate Time to Crack a Password"]
    for x in range(len(menu)):
        print(menu[x])
    usrInput = input("> ")
    if(compare.compare(menu[0], usrInput)): # see stringComparison, ignore array defaults to empty
        ht = input("sha512 md5\nHash Type: ")
        dt = input("regular hex\nDigest Type: ")
        string = input("Phrase to be hashed: ")
        salt = input("Salt: ")
        print(str(makeHash(ht, dt, string, salt)))
    elif(compare.compare(menu[1], usrInput, ["password", "for"])):
        solve()
    elif(compare.compare(menu[2], usrInput, ["to", "a", "password"])):
        pswd = input("Plaintext password: ")
        pps = int(input("CPU speed (passwords/second): "))
        timeToCrack(pswd, pps)
def makeHash(hashType, digestType, string, salt):
    result = ""
    if(hashType == "sha512" or hashType == "SHA512"):
        if(digestType == "hex"):
            result = hashlib.sha512((string+salt).encode()).hexdigest()
        else:
            result = hashlib.sha512((string+salt).encode()).digest()
    elif(hashType == "md5" or hashType == "MD5"):
        if(digestType == "hex"):
            result = hashlib.md5((string+salt).encode()).hexdigest()
        else:
            result = hashlib.md5((string+salt).encode()).digest()
    return result
def solve():
    hashType = input("sha512 md5\nHash type: ")
    digest = input("regular hex\nDigest type: ")
    passHash = input("Hash: ")
    salt = input("Hash salt: ")
    '''start = datetime.datetime.now()
    end = datetime.datetime.now()
    hour = end.hour - start.hour
    minute = end.minute - start.minute
    second = end.second - start.second
    print("Process took " + str(hour) + " hours, " + str(minute) + " minutes, and " + str(second) + " seconds to complete")
    '''
    threadsCount = int(input("Number of threads desired: ")) # *** THIS SEGMENT WAS TOUCHED UP 2017
    threadsMin = int(input("Minimum number of characters: ")) # prior I had used threads and simply designated the threads as 1-5 and 5-7
    threadsMax = int(input("Maximum number of characters expected: "))+1 # because the upper bound is not inclusive
    threadsCount = min(threadsCount, threadsMax-threadsMin) # in case there are more threads than would be useful
    minMax = []
    last = 0
    for x in range(threadsCount):
        minMax.append([threadsMin+last, threadsMax-threadsCount+x+1])
        last = threadsMax-(threadsCount+threadsMin)+x+1 # 2017 EDITING ENDS HERE ***
    threads = []
    for x in range(len(minMax)):
        threads.append(cycleThread(x, minMax[x][0], minMax[x][1], passHash, salt, c,  hashType, digest))
    for x in range(len(minMax)): # separated to pass all threads to first thread
        threads[x].threads = threads
        threads[x].start()
def timeToCrack(plainText, passwordsPerSecond): # *** THIS FUNCTION WAS TOUCHED UP 2017 ***
    bruteForce = len(c)**(len(plainText))
    finesse = bruteForce*(c.index(plainText[0])/len(c)) #it only has to go that fraction of the way through the brute force
    seconds = finesse/passwordsPerSecond
    minutes = seconds/60
    hours = minutes/60
    days = hours/24
    years = days/365
    print("{0:.4f} years, or {1:.4f} days, or {2:.4f} hours, or {3:.4f} minutes, in Big O Notation (worst case senario)".format(years, days, hours, minutes))

main()
