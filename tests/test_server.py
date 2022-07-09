"""test_server.py

Script to test the server.
"""

import unittest

class TestServer(unittest.TestCase):
    """Class to test the functionality of the powerbank server."""

    def setUp(self):
        pass

    def tearDown(self):
        pass

    def test_connection(self):
        """Try to make sucessful tcp connection to the server."""

    def test_send_data(self):
        """Check to see if the server recieved the data."""

    def test_log_data(self):
        """Check to see if the server logs received data."""

    def test_make_log(self):
        """Check to see if the server makes a new log file if
        the log file is missing."""

    def test_bad_request(self):
        """Check to see if the serve handles bad requests."""

if __name__ == '__main__':
    unittest.main()
