#!/bin/bash
#################################################
# unit tests TODO:
# storing so many accounts that printing them exceeds page size


# test pwkeeper_master_write:
# (setting master password for users)
function test1() {
    echo "PERFORMING TEST1"
    echo -n '${USER}a' > /dev/pwkeeper_master
    echo -n '${USER}b' > /dev/pwkeeper_master

    sudo -u test1 echo -n 'test1a' > /dev/pwkeeper_master
    sudo -u test2 echo -n 'test2a' > /dev/pwkeeper_master
    sudo -u test3 echo -n 'test3a' > /dev/pwkeeper_master

    sudo -u test1 echo -n 'test1b' > /dev/pwkeeper_master
    sudo -u test2 echo -n 'test3b' > /dev/pwkeeper_master

    #################################################
    # test pwkeeper_master_write:
    # (adding accounts for user(s))
    echo -n '1-google.com' > /dev/pwkeeper_account
    echo -n '1-yahoo.com' > /dev/pwkeeper_account
    echo -n '1-gmail.com' > /dev/pwkeeper_account
    echo -n '1-goog' > /dev/pwkeeper_account
    echo -n '1-example.com' > /dev/pwkeeper_account


    sudo -u test1 echo -n 'google.com' > /dev/pwkeeper_account
    sudo -u test1 echo -n 'yahoo.com' > /dev/pwkeeper_account
    sudo -u test2 echo -n 'google.com' > /dev/pwkeeper_account
    sudo -u test2 echo -n 'goog' > /dev/pwkeeper_account
    sudo -u test2 echo -n 'reddit.com' > /dev/pwkeeper_account
    sudo -u test2 echo -n 'google.com' > /dev/pwkeeper_account
}


function test2() {
    echo "PERFORMING TEST2"
    # test provided at end of part 3 (expected password is "UMWd8^TH\]RkTXLR")
    sudo -u test4  echo -n '01234567890123456789012345678901' > /dev/pwkeeper_master
    sudo -u test4 echo -n 'abcdEFGHijklMNOP' > /dev/pwkeeper_account
}

function test3() {
    echo "PERFORMING TEST3"
    # create 2 master accounts
    sudo -u test1 echo -n 'test1a' > /dev/pwkeeper_master # 1001
    sudo -u test2 echo -n 'test2a' > /dev/pwkeeper_master # 1002

    # create (site) accounts for each
    sudo -u test1 echo -n 'google.com' > /dev/pwkeeper_account
    sudo -u test1 echo -n 'yahoo.com' > /dev/pwkeeper_account

    sudo -u test2 echo -n 'reddit.com' > /dev/pwkeeper_account
    sudo -u test2 echo -n 'youtube.com' > /dev/pwkeeper_account

    # update master password of test2
    # (ensure site passwords are updated)
    sudo -u test1 echo -n 'test2b' > /dev/pwkeeper_master
}

function test4() {
    # test viewing all accounts (for a given user)
    echo "PERFORMING TEST4"
    # create 2 users
    echo -n 'cs421_rocks!' > /dev/pwkeeper_master
    sudo -u test1 echo -n 'umbc_rocks!' > /dev/pwkeeper_master

    echo -n 'umbc.edu' > /dev/pwkeeper_account
    sudo -u test1 echo -n 'INVISIBLE.COM' > /dev/pwkeeper_account
    echo -n 'gmail.com' > /dev/pwkeeper_account
    echo -n 'snapchat.com' > /dev/pwkeeper_account
    echo -n '1234567890123456' > /dev/pwkeeper_account
    cat /sys/devices/platform/pwkeeper/accounts

    sudo -u test1 cat /sys/devices/platform/pwkeeper/accounts

    # test viewing all users
    sudo -u test3 echo -n 'maryland rocks!' > /dev/pwkeeper_master
    cat /sys/devices/platform/pwkeeper/masters
    sudo cat /sys/devices/platform/pwkeeper/masters
}

function test5() {
    # test what happens when a user has MANY accounts (and would exceed PAGE_SIZE)
    echo "PERFORMING TEST5"
    echo -n 'cs421_rocks!' > /dev/pwkeeper_master
    for i in {1..1000};
    do
        #echo "example${i}.com"
        echo -n "example${i}.com" > /dev/pwkeeper_account
    done
    # try to list all accounts
    cat /sys/devices/platform/pwkeeper/accounts
}

function test6() {
    # see bottom of part 3 for expected output
    echo -n '01234567890123456789012345678901' > /dev/pwkeeper_master
    echo -n 'abcdEFGHijklMNOP' > /dev/pwkeeper_account
    cat /dev/pwkeeper_account && echo
}

test6
