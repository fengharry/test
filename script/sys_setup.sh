#!/bin/bash

TAG="----"
VIMRC=/usr/share/vim/vim74/vimrc_example.vim
SMBCONF=/etc/samba/smb.conf
RED="\033[0;31m"
GREEN="\033[0;32m"
NC="\033[0m"

# if $1=1 then update software list
if [ "$1"x = "1"x ]; then
	sudo apt-get update
fi

sudo apt-get install aptitude vim vim-gtk terminator bless

# if $2=1 then copy the vimrc file
if [ "$2"x = "1"x ]; then
	if [ -f $VIMRC ]; then
		echo -e "$GREEN $TAG copy the vimrc $NC"
		cp $VIMRC ~/.vimrc
		# set vimrc
		echo "set number" >>~/.vimrc
		echo "set tabstop=4" >>~/.vimrc
		echo "set shiftwidth=4" >>~/.vimrc
		echo "set smarttab" >>~/.vimrc
		echo "set smartindent" >>~/.vimrc
	fi
fi

sudo apt-get install cmake g++ git xclip zlib1g-dev libboost-all-dev samba ibus-table-wubi openssh-server default-jdk \
	shutter meld

# configure git
git config --global user.email "fengharry126@126.com"
git config --global user.name "harry"
git config --global core.excludesfile ~/.gitignore_global
git config --global diff.tool meld
git config --global merge.tool meld
cat > ~/.gitignore_global << EOF
# Object files
*.o

# Shared objects
*.so

# eclipse files
.cproject
.project

# debug files
debug
build
EOF

# add samba share folder
if [ "$3"x = "1"x ]; then
	if [ -w $SMBCONF ]; then
		echo -e "$GREEN $TAG smb.conf already modified $NC"
	else
		sudo chmod 666 $SMBCONF
		if [ -d /home/harry/share ]; then
			echo -e "$GREEN $TAG /home/harry/share already exist $NC"
		else
			mkdir /home/harry/share
		fi
		chmod 777 /home/harry/share
		echo "[share]" >> $SMBCONF
		echo "  path = /home/harry/share" >> $SMBCONF
		echo "  guest ok = yes" >> $SMBCONF
		echo "  browseable = yes" >> $SMBCONF
		echo "  writable = yes" >>$SMBCONF
	fi
fi

if [ "$4"x = "1"x ]; then
	wget http://mirrors.neusoft.edu.cn/eclipse/technology/epp/downloads/release/neon/3/eclipse-cpp-neon-3-linux-gtk-x86_64.tar.gz -P ~/Downloads/
fi
