import subprocess

def compute_laziest_best_global_path(phone_number:str):
  return subprocess.call(["./build/PhoneNumber", phone_number])
