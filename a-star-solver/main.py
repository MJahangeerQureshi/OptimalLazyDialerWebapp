import subprocess

number = input("Enter the phone number: ")
subprocess.call(["./build/PhoneNumber", str(number)])