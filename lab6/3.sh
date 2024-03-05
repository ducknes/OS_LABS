#!/bin/bash

variant=1

while true; do
	echo "Меню: "
	echo "1. Поиск файлов изменённых 1 день назад"
	echo "2. Поиск процессов пользователя"
	echo "0. Выход"

	read -p "Введите номер пункта меню: " switch

	case $switch in
		1)
			echo "Поиск файлов изменённых $variant назад"
			find ~/ -type f -mtime -$variant > "out$variant.$$" 2> "err$variant.$$";;
		2)
			read -p "Введите имя пользователя: " username
			ps -u "$username" | sort > "proc.$username";;
		0)
			echo "Выход"
			exit 0;;
		*)
			echo "Выбран не корректный пункт меню";;
	esac
done

