import re
import subprocess
import socket
import sys
import time
import threading


class ForwardCAN(object):
    '''
    Sukuria can srauto iteratorius
    '''
    def __init__(self, port, fake):
        self.timeout = 3
        self.fake = fake
        #self.leases_file = "dhcpd.leases"
        self.leases_file = "/var/lib/dhcp/dhcpd.leases"
        self.leases_ip_regex = r"^lease ([0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3})"
        try:
            self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.port = port
            self.connect()
        except socket.timeout:
            print "Timeout"

    def parse_CAN(self, line):
        '''
        laukiamas formatas: can0 410 [8] 0E 08 00 00 00 6D 00 00
        '''
        tokens = line.split() #tarpai dingsta
        address = tokens[1]
        dlc = tokens[2][1:-1] #ignoruojami lauztiniai skliaustai
        data = ""
        for d in xrange(3, 3 + int(dlc)):
            data += tokens[d]

        return "#" + address + "#" + dlc + "#" + data + "#"

    def connect(self):
        try:
            ip = None
            while ip == None:
                ip = self.get_ip()
                time.sleep(2)

            address = (ip, self.port)
            self.sock.connect(address)
        except socket.error:
            print "Socket error"
            time.sleep(2)
            self.connect()

    def send(self, rxcan, data_stream):
        try:
            data = self.parse_CAN(next(data_stream))
            print "Sending: " + data
            self.sock.send(data) # next - iteratoriaus metodas gauti sekancius duomenis

        except StopIteration:
            if self.fake:
                print "Kazkas negerai, fake neturi sustoti"
            rxcan.reset_can_interface() # jeigu pastringa CAN interfeisas
            time.sleep(2)
        except socket.error:
            print "Socket error"
            self.connect()
            time.sleep(2)

    def send_can(self, retrieve_can):
        if self.fake:
            data_stream = retrieve_can.get_fake_stream()
        else:
            data_stream = retrieve_can.get_can_stream()

        while True: #siuncia be galo
            thread = threading.Thread(target=self.send, args=(retrieve_can, data_stream))
            thread.start()
            thread.join(self.timeout)


    def check_ip(self, ip):
        output = subprocess.Popen(["ping -qc 3 " + ip + " && echo $?"],
                                  shell=True,
                                  stdout=subprocess.PIPE).communicate()[0]
        lines = output.split('\n')
        if lines[len(lines) - 2] == '0':
            print "{0} is up".format(ip)
            return True
        else:
            print "{0} is down".format(ip)
            return False

    def get_ip(self):
        '''
        Gauna IP, kuriam reikia transliuoti CAN srauta
        Grazina Dotted decimal formatu
        '''
        with open(self.leases_file) as f:
            for line in f:
                match = re.match(self.leases_ip_regex, line, re.M|re.I)
                if match and self.check_ip(match.group(1)):
                    print "Found active IP: " + match.group(1)
                    return match.group(1)

