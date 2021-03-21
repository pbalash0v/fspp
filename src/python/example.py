#!/usr/bin/env python

import sys
sys.path.append('../lib')

import os
import multiprocessing

import ESL
import pyfspp


class FreeSWITCH(multiprocessing.Process):
	def run(self):
		sys.stdout = open(os.devnull, 'w')
		sys.stderr = open(os.devnull, 'w')
		pyfspp.FSPP()()
	

def get_esl_connection():
	while True:
		con = ESL.ESLconnection("::1", 8021, "ClueCon")
		if con.connected():
			return con

def get_fs_version(esl_conn):
	e = esl_conn.api("version")
	return e.getBody()

def shutdown_fs(esl_conn):
	e = esl_conn.api("shutdown")
	return e.getBody()

def main():
	p = FreeSWITCH()
	p.start()
	esl_conn = get_esl_connection()
	fs_version = get_fs_version(esl_conn)
	shutdown_fs(esl_conn)
	p.join()
	print '\n',fs_version


if __name__ == '__main__':
	main()