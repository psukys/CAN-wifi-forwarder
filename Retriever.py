import subprocess

class FakeCAN:
    def __init__(self):
        self.data = ["can0 321 [8] 40 15 80 01 FF 60 05 60",
                     "can0 322 [8] 40 15 80 01 04 60 05 60",
                     "can0 323 [8] 40 15 80 01 04 60 05 60",
                     "can0 325 [8] 40 15 80 01 04 60 05 60",
                     "can0 326 [8] 15 15 80 01 04 60 05 60",
                     "can0 7D1 [8] 40 9C 00 01 00 55 05 60",
                     "can0 7D2 [8] FF E2 08 01 04 60 05 60",
                     "can0 4B3 [8] 40 50 06 08 55 01 02 03",
                     "can0 7D2 [8] 40 15 D8 01 04 60 05 60",
                     "can0 321 [8] 40 05 60 80 09 60 05 60",
                     "can0 340 [8] 04 05 60 80 09 60 05 60",
                     "can0 341 [8] 04 05 60 80 09 60 05 60",
                     "can0 342 [8] 04 05 60 80 09 60 05 60",
                     "can0 343 [8] 04 05 60 80 09 60 05 60",
                     "can0 344 [8] 04 05 60 80 09 60 05 60",
                     "can0 345 [8] 04 05 60 80 09 60 05 60",
                     "can0 360 [8] 04 05 60 80 09 60 05 60",
                     "can0 361 [8] 04 05 60 80 09 60 05 60",
                     "can0 362 [8] 04 05 60 80 09 60 05 60",
                     "can0 363 [8] 04 05 60 80 09 60 05 60",
                     "can0 364 [8] 04 05 60 80 09 60 05 60",
                     "can0 365 [8] 04 05 60 80 09 60 05 60",
                     "can0 4B3 [8] 04 05 60 80 09 60 05 03",
                     "can0 7D1 [8] 40 C8 02 02 04 11 05 60"]
        self.index = 0

    def __iter__(self):
        return self

    def next(self):
        if self.index == len(self.data):
            self.index = 0
        next_data = self.data[self.index]
        self.index += 1
        return next_data

class RetrieveCAN:
    def get_fake_stream(self):
        fk = FakeCAN()
        return fk

    def get_can_stream(self):
        command = ["candump", "can0,0:0,#FFFFFFFF"]
        process = subprocess.Popen(command, stdout=subprocess.PIPE)
        return iter(process.stdout.readline, '')

    def reset_can_interface(self):
        for i in range(5):
            p = subprocess.Popen(["./setup.sh"], stdout=subprocess.PIPE).communicate()[0]

            if p == 0:
                return True
            else:
                return False
