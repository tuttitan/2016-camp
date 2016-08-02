#!/usr/bin/python
# -*- coding: utf-8 -*-

# ********************************
# ***** 点数表連携スクリプト *****
# ********************************

import socket
import random
import sys


HOST_ADDR = ''        # INADDR_ANYで接続を受ける
PORT = 4000           # ポート番号
BUF_SIZE = 1024       # バッファサイズ

def main():

    bQuit = False;    # 終了フラグ
    points = []
    points_init = []
    
    if 1 < len(sys.argv) :
        host_addr = sys.argv[1]
        # debug
        print "Wait for Connection from " + str(host_addr) + " ..."
    else :
        host_addr = ''
        print "Wait for Connection INADDR_ANY"

    # 通信を開始する
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.bind((host_addr, PORT))
    #sock.bind((HOST_ADDR, PORT))
    sock.listen(1)

    (conn, address) = sock.accept()
    print "Connect by " + str(address)
    
    while bQuit != True :
        msg = conn.recv(BUF_SIZE)
        if 'init' == msg :
            print "Initilize Score"
            
            del points_init[:]
            
            # 初期ポイントとSeedを受信する
            msg = conn.recv(BUF_SIZE)
            split_msg = msg.split(":")
            msg_pt = split_msg[0];    # 初期ポイント
            msg_sd = split_msg[1];    # 乱数Seed
            
            # 乱数の初期化
            random.seed(int(msg_sd))
            
            print "Points = " + str(msg_pt)
            print "Seed   = " + str(msg_sd)
            
            split_msg = msg_pt.split(",")
            for i in range(0, len(split_msg)) :
                p = int(split_msg[i])
                points_init.append(p)
            
            print "points: " + str(points_init)
            
            points = points_init[:]

        elif 'update' == msg :
            print "Update Score"
            random.shuffle(points)
            print "points: " + str(points)
            
            # 更新されたポイントを送信する
            conn.send(str(points))
            
        elif 'quit' == msg:
            bQuit = True;
            print "Quit Script..."
        else :
            print "Other case " + str(msg)
    conn.close();
    sock.close();

if __name__ == '__main__':
  main()