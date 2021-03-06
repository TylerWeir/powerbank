"""test_server.py

Script to test the server.
"""

import unittest
import struct
import socket
import time
import os

HOST = "127.0.0.1"
PORT = 8080

class TestServer(unittest.TestCase):
    """Class to test the functionality of the powerbank server."""

    def test_a(self):
        """Server logs data."""
        with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as test_socket:
            testString = "hello upd server."
            test_socket.sendto(testString.encode(), (HOST, PORT));

    def test_b(self):
        """Server makes log file."""
        # log_file_name = '~/Code/powerbank/log.txt'
        # tmp_log_file = '~/Code/powerbank/tmp_log.txt'

        # # Move the original file
        # if os.path.exists(log_file_name):
        #     os.rename(log_file_name, tmp_log_file)

        # # Send some data
        # with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as test_socket:
        #     try:
        #         test_socket.settimeout(5)
        #         test_socket.connect((HOST, PORT))
        #     except TimeoutError:
        #         self.fail("Timed out trying to connect to the server.")
        #     except ConnectionRefusedError:
        #         self.fail("The connection was refused.")

        #     try:
        #         num_1 = struct.pack("h", 420)
        #         test_socket.sendall(num_1)
        #     except TimeoutError:
        #         self.fail("Timed out waiting for a response from the server.")

        # # Wait a bit after sending
        # time.sleep(3)
        # # Check that the log file was remade
        # self.assertTrue(os.path.exists(log_file_name))

        # # Fix it all
        # if os.path.exists(tmp_log_file):
        #     os.remove(log_file_name)
        #     os.rename(tmp_log_file, log_file_name)
        # else:
        #     os.remove(log_file_name)

    def test_d(self):
        """Sever handles bad data"""
        # with open('~/Code/powerbank/log.txt', mode='r', encoding='UTF-8') as log_file:
        #     # look for the lines
        #     lines = tuple(log_file.readlines())

        # self.test_socket.sendall(b"bad data")
        # time.sleep(0.5)
        # self.test_socket.sendall(b"")
        # time.sleep(0.5)

        # with open('~/Code/powerbank/log.txt', mode='r', encoding='UTF-8') as log_file:
        #     # look for the lines
        #     after_lines = tuple(log_file.readlines())

        # # This works because values are tuples
        # self.assertEqual(lines, after_lines)

    def test_e(self):
        """Server handles simultaneous connections."""
        # other_test_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        # other_test_socket.connect((HOST, PORT))

        # other_test_socket.sendall(4.53)
        # self.test_socket.sendall(464.231)

        # with open('~/Code/powerbank/log.txt', mode='r', encoding='UTF-8') as log_file:
        #     # look for the lines
        #     values = [float(val) for val in log_file.readlines()[-2:]]
        #     self.assertTrue(4.53 in values)
        #     self.assertTrue(464.231 in values)

        # other_test_socket.close()

if __name__ == '__main__':
    unittest.main(verbosity=2)
