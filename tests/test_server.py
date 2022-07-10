"""test_server.py

Script to test the server.
"""

import unittest
import socket
import time
import os

HOST = "127.0.0.1"
PORT = 65432

class TestServer(unittest.TestCase):
    """Class to test the functionality of the powerbank server."""

    def setUp(self):
        self.test_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    def tearDown(self):
        self.test_socket.close()

    def test_connection(self):
        """Connect to server."""
        try:
            self.test_socket.connect((HOST, PORT))
        except TimeoutError:
            self.fail("Timed out trying to connect to the server")
        except ConnectionRefusedError:
            self.fail("The connection was refused")

    def test_send_data(self):
        """Send data to server."""
        self.test_socket.sendall(b"Hello there!")
        data = self.test_socket.recv(1024)
        self.assertEqual(data, "General Kenobi.")

    def test_log_data(self):
        """Server logs data."""
        # Send the data
        self.test_socket.sendall(2.031)
        time.sleep(1)
        self.test_socket.sendall(3.532)
        time.sleep(1)
        self.test_socket.sendall(54.334)

        with open('~/Code/powerbank/log.txt', mode='r', encoding='UTF-8') as log_file:
            # look for the lines
            lines = log_file.readlines()
            self.assertEqual(2.031, float(lines[-3]))
            self.assertEqual(3.532, float(lines[-2]))
            self.assertEqual(54.334, float(lines[-1]))

            # delete the lines
            log_file.seek(0)
            for i, value in enumerate(lines):
                if i < len(lines)-3:
                    log_file.write(value)
            log_file.truncate()

    def test_make_log(self):
        """Server makes log file."""
        log_file_name = '~/Code/powerbank/log.txt'

        # Move the original file
        if os.path.exists(log_file_name):
            os.rename(log_file_name, '~/Code/powerbank/tmp_log.txt')

        # Send some data
        self.test_socket.sendall(23.252)
        time.sleep(0.5)

        # Check that the log file was remade
        self.assertTrue(os.path.exists(log_file_name))

        # Fix it all
        os.remove(log_file_name)
        os.rename('~/Code/powerbank/tmp_log.txt', log_file_name)

    def test_bad_request(self):
        """Sever handles bad data"""
        with open('~/Code/powerbank/log.txt', mode='r', encoding='UTF-8') as log_file:
            # look for the lines
            lines = tuple(log_file.readlines())

        self.test_socket.sendall(b"bad data")
        time.sleep(0.5)
        self.test_socket.sendall(b"4f.30")
        time.sleep(0.5)

        with open('~/Code/powerbank/log.txt', mode='r', encoding='UTF-8') as log_file:
            # look for the lines
            after_lines = tuple(log_file.readlines())

        # This works because values are tuples
        self.assertEqual(lines, after_lines)

    def test_multiple_connections(self):
        """Server handles simultaneous connections."""
        other_test_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        other_test_socket.connect((HOST, PORT))

        other_test_socket.sendall(4.53)
        self.test_socket.sendall(464.231)

        with open('~/Code/powerbank/log.txt', mode='r', encoding='UTF-8') as log_file:
            # look for the lines
            values = [float(val) for val in log_file.readlines()[-2:]]
            self.assertTrue(4.53 in values)
            self.assertTrue(464.231 in values)

        other_test_socket.close()

if __name__ == '__main__':
    unittest.main()
