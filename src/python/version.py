#!/usr/bin/env python

import os
import multiprocessing

import ESL
import pyfspp



def run_fs(cfg):
	fs = pyfspp.FSPP(cfg)
	fs()


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
	cfg = pyfspp.config()
	p = multiprocessing.Process(target=run_fs, args=(cfg,))
	print 'Starting FreeSWITCH...'
	p.start()

	try:
		print 'Opening ESL connection...'
		esl_conn = get_esl_connection()
		print 'ESL connected'
		fs_version = get_fs_version(esl_conn)
		print fs_version
		print 'Shutting down FreeSWITCH...'
		shutdown_fs(esl_conn)
		p.join()
	except KeyboardInterrupt:
		p.terminate()


if __name__ == '__main__':
	main()



