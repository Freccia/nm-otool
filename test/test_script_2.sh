#!/bin/sh

make -C ../
rm -f their mine mine_err their_err 2>/dev/null
TESTNM=1
TESTOTOOL=1

if [ $TESTNM != 0 ]; then
	echo "Test NM 32bits on 32/:"
	find "./32" -type f -print0 | \
	while IFS='' read -r -d '' filename; do
		[ $? == 1 ] && exit 0;
		FILE1=$filename
		nm $FILE1 > their ; ../ft_nm $FILE1 > mine
		COUNT=$(diff their mine | wc -l)
		if [ $COUNT != 0 ]; then
			echo "32bits test NM : FAILED! : "$FILE1": see their, mine."
			exit 1
		else
			echo "32bits test NM : SUCCESS! : "$FILE1
		fi
	done
	[ $? == 1 ] && exit 0;
	#without fat files
	echo "Test NM 64bits binary on /bin:"
	find "/bin" -type f -print0 | \
	while IFS='' read -r -d '' filename; do
		FILE1=$filename
		[ $? == 1 ] && exit 0;
		if [ "$FILE1" != "/bin/bash" -a "$FILE1" != "/bin/sh" -a "$FILE1" != "/bin/sync" ]; then
			nm $FILE1 > their ; ../ft_nm $FILE1 > mine
			COUNT=$(diff their mine | wc -l)
			if [ $COUNT != 0 ]; then
				echo "64bits tests NM: FAILED! : "$FILE1": see their, mine."
				exit 1
			else
				echo "64bits tests NM: SUCCESS! :"$FILE1
			fi
		fi
	done
	[ $? == 1 ] && exit 0;
	echo "Test NM fat files on /bin:"
	nm /bin/bash > their ; ../ft_nm /bin/bash > mine
	COUNT=$(diff their mine | wc -l)
	if [ $COUNT != 0 ]; then
		echo "Fat tests NM: FAILED! : /bin/bash: see their, mine."
		exit 1
	else
		echo "Fat tests NM: SUCCESS! : /bin/bash"
	fi
	[ $? == 1 ] && exit 0;
	nm /bin/sh > their ; ../ft_nm /bin/sh > mine
	COUNT=$(diff their mine | wc -l)
	if [ $COUNT != 0 ]; then
		echo "Fat tests NM: FAILED! : /bin/sh: see their, mine."
		exit 1
	else
		echo "Fat tests NM: SUCCESS! : /bin/sh"
	fi
	[ $? == 1 ] && exit 0;
	nm /bin/sync > their ; ../ft_nm /bin/sync > mine
	COUNT=$(diff their mine | wc -l)
	if [ $COUNT != 0 ]; then
		echo "Fat tests: FAILED! : /bin/sync: see their, mine."
		exit 1
	else
		echo "Fat tests: SUCCESS! : /bin/sync"
	fi
	[ $? == 1 ] && exit 0;
	echo "Test NM archives 32 & 64:"
	find "./ar" -type f -print0 | \
	while IFS='' read -r -d '' filename; do
		FILE1=$filename
		nm $FILE1 > their ; ../ft_nm $FILE1 > mine
		COUNT=$(diff their mine | wc -l)
		if [ $COUNT != 0 ]; then
			echo "Archive tests NM: FAILED! : "$FILE1": see their, mine."
			exit 1
		else
			echo "Archive tests NM: SUCCESS! :"$FILE1
		fi
	done

	#test on generic lib, some ignored
	#with cat to ignore error due to nm sort order with same string
	# libkmodc++ ,cplus_start.o): cplus_stop.o): return 2 error : no name list, mine : archive size detected == 0, no entry found in archive
	# erreur /usr/lib/libnetsnmp.5.2.1.dylib detecte comme i386, nm /usr/lib/libnetsnmp.5.2.1.dylib(link editor): ???
	[ $? == 1 ] && exit 0;
	echo "Test NM on /usr/lib:"
	find "/usr/lib" -type f -print0 | \
	while IFS='' read -r -d '' filename; do
		FILE1=$filename
		[ $? == 1 ] && exit 0;
		if [ "$FILE1" != "" -a "$FILE1" != "/usr/lib/libnetsnmp.5.2.1.dylib" ]; then
			nm $FILE1 2>/dev/null > their 
			../ft_nm  $FILE1 2>/dev/null > mine
			COUNT=$(cat their mine | sort | uniq -u | wc -l)
			if [ $COUNT != 0 ]; then
				echo "/usr/lib tests NM: FfAILED! : "$FILE1": see their, mine."
				exit 1;
			else
				echo "/usr/lib tests NM: SUCCESS! :"$FILE1
			fi
		fi
	done

	[ $? == 1 ] && exit 0;
	#access denied sur /sbin/yubikey_shell
	echo "Test NM on /sbin:"
	find "/sbin" -type f -print0 | \
	while IFS='' read -r -d '' filename; do
		FILE1=$filename
		[ $? == 1 ] && exit 0;
		if [ "$FILE1" != "/usr/lib/libnetsnmp.5.2.1.dylib" ]; then
			nm $FILE1 2> their_err  > their  
			../ft_nm $FILE1  2> mine_err > mine
			COUNT=$(cat their mine | sort | uniq -u | wc -l)
			COUNTET=$(cat their_err | grep "Permission denied" | wc -l)
			COUNTEM=$(cat mine_err | grep "Permission denied" | wc -l)
			if [ $COUNT != 0 ]; then
				echo "/sbin tests NM: FAILED! : "$FILE1": see their, mine."
				exit 1
			else
				echo "/sbin tests NM: SUCCESS! :"$FILE1
			fi
			if [ $COUNTET != $COUNTEM ]; then
				exit 1
			fi
		fi
	done
	[ $? == 1 ] && exit 0;
	echo "Test NM on /usr/bin:"
	find "/usr/bin" -type f -print0 | \
	while IFS='' read -r -d '' filename; do
		FILE1=$filename
		nm $FILE1 2> their_err  > their  
		../ft_nm $FILE1  2> mine_err > mine
		COUNT=$(cat their mine | sort | uniq -u | wc -l)
		if [ $COUNT != 0 ]; then
			echo "/usr/bin tests NM: FAILED! : "$FILE1": see their, mine."
			exit 1
		else
			echo "/usr/bin tests NM: SUCCESS! :"$FILE1
		fi
	done

	echo "Test NM on /usr/sbin:"
	find "/usr/sbin" -type f -print0 | \
	while IFS='' read -r -d '' filename; do
		[ $? == 1 ] && exit 0;
		FILE1=$filename
		nm $FILE1 2> their_err  > their  
		../ft_nm $FILE1  2> mine_err  > mine
		COUNT=$(cat their mine | sort | uniq -u | wc -l)
		if [ $COUNT != 0 ]; then
			echo "/usr/sbin tests NM: FAILED! : "$FILE1": see their, mine."
			exit 1
		else
			echo "/usr/bin tests NM: SUCCESS! :"$FILE1
		fi
	done
fi

if [ $TESTOTOOL != 0 ]; then
	echo "Test OTOOL 32bits on 32/:"
	find "./32" -type f -print0 | \
	while IFS='' read -r -d '' filename; do
		[ $? == 1 ] && exit 0;
		FILE1=$filename
		otool -t $FILE1 > their ; ../ft_otool $FILE1 > mine
		COUNT=$(diff their mine | wc -l)
		if [ $COUNT != 0 ]; then
			echo "32bits test OTOOL : FAILED! : "$FILE1": see their, mine."
			exit 1
		else
			echo "32bits test OTOOL : SUCCESS! : "$FILE1
		fi
	done
	[ $? == 1 ] && exit 0;
	#without fat files
	echo "Test OTOOL 64bits binary on /bin:"
	find "/bin" -type f -print0 | \
	while IFS='' read -r -d '' filename; do
		FILE1=$filename
		[ $? == 1 ] && exit 0;
		if [ "$FILE1" != "/bin/bash" -a "$FILE1" != "/bin/sh" -a "$FILE1" != "/bin/sync" ]; then
			otool -t $FILE1 > their ; ../ft_otool $FILE1 > mine
			COUNT=$(diff their mine | wc -l)
			if [ $COUNT != 0 ]; then
				echo "Standard tests OTOOL: FAILED! : "$FILE1": see their, mine."
				exit 1
			else
				echo "Standard tests OTOOL: SUCCESS! :"$FILE1
			fi
		fi
	done
	[ $? == 1 ] && exit 0;
	echo "Test OTOOL fat files on /bin:"
	otool -t /bin/bash > their ; ../ft_otool /bin/bash > mine
	COUNT=$(diff their mine | wc -l)
	if [ $COUNT != 0 ]; then
		echo "Fat tests OTOOL: FAILED! : /bin/bash: see their, mine."
		exit 1
	else
		echo "Fat tests OTOOL: SUCCESS! : /bin/bash"
	fi
	[ $? == 1 ] && exit 0;
	otool -t /bin/sh > their ; ../ft_otool /bin/sh > mine
	COUNT=$(diff their mine | wc -l)
	if [ $COUNT != 0 ]; then
		echo "Fat tests OTOOL: FAILED! : /bin/sh: see their, mine."
		exit 1
	else
		echo "Fat tests OTOOL: SUCCESS! : /bin/sh"
	fi
	[ $? == 1 ] && exit 0;
	otool -t /bin/sync > their ; ../ft_otool /bin/sync > mine
	COUNT=$(diff their mine | wc -l)
	if [ $COUNT != 0 ]; then
		echo "Fat tests OTOOL: FAILED! : /bin/sync: see their, mine."
		exit 1
	else
		echo "Fat tests OTOOL: SUCCESS! : /bin/sync"
	fi
	[ $? == 1 ] && exit 0;
	echo "Test OTOOL archives 32 & 64:"
	find "./ar" -type f -print0 | \
	while IFS='' read -r -d '' filename; do
		FILE1=$filename
		otool -t $FILE1 > their ; ../ft_otool $FILE1 > mine
		COUNT=$(diff their mine | wc -l)
		if [ $COUNT != 0 ]; then
			echo "Archive tests OTOOL: FAILED! : "$FILE1": see their, mine."
			exit 1
		else
			echo "Archive tests OTOOL: SUCCESS! :"$FILE1
		fi
	done

	#test on generic lib, some ignored
	#with cat to ignore error due to nm sort order with same string
	# libkmodc++ ,cplus_start.o): cplus_stop.o): return 2 error : no name list, mine : no entry foud in archive
	# erreur /usr/lib/libnetsnmp.5.2.1.dylib detecte comme i386, nm ???
	[ $? == 1 ] && exit 0;
	echo "Test OTOOL on /usr/lib:"
	find "/usr/lib" -type f -print0 | \
	while IFS='' read -r -d '' filename; do
		FILE1=$filename
		[ $? == 1 ] && exit 0;
		if [ "$FILE1" != "/usr/lib/libkmodc++.a" -a "$FILE1" != "/usr/lib/libnetsnmp.5.2.1.dylib" ]; then
			otool -t $FILE1 2>/dev/null > their 
			../ft_otool  $FILE1 2>/dev/null > mine
			COUNT=$(cat their mine | sort | uniq -u | wc -l)
			if [ $COUNT != 0 ]; then
				echo "/usr/lib tests OTOOL: FfAILED! : "$FILE1": see their, mine."
				exit 1;
			else
				echo "/usr/lib tests OTOOL: SUCCESS! :"$FILE1
			fi
		fi
	done

	[ $? == 1 ] && exit 0;
	#access denied sur /sbin/yubikey_shell
	echo "Test OTOOL on /sbin:"
	find "/sbin" -type f -print0 | \
	while IFS='' read -r -d '' filename; do
		FILE1=$filename
		[ $? == 1 ] && exit 0;
		if [ "$FILE1" != "/usr/lib/libnetsnmp.5.2.1.dylib" ]; then
			otool -t $FILE1 2> their_err > their  
			../ft_otool $FILE1  2> mine_err  > mine
			COUNT=$(cat their mine | sort | uniq -u | wc -l)
			COUNTET=$(cat their_err | grep "Permission denied" | wc -l)
			COUNTEM=$(cat mine_err | grep "Permission denied" | wc -l)
			if [ $COUNT != 0 ]; then
				echo "/sbin tests OTOOL: FAILED! : "$FILE1": see their, mine."
				exit 1
			else
				echo "/sbin tests OTOOL: SUCCESS! :"$FILE1
			fi
			if [ $COUNTET != $COUNTEM ]; then
				exit 1
			fi
		fi
	done
	[ $? == 1 ] && exit 0;
	echo "Test on /usr/bin:"
	find "/usr/bin" -type f -print0 | \
	while IFS='' read -r -d '' filename; do
		FILE1=$filename
		otool -t $FILE1 2> their_err  > their  
		../ft_otool $FILE1  2> mine_err  > mine
		COUNT=$(cat their mine | sort | uniq -u | wc -l)
		if [ $COUNT != 0 ]; then
			echo "/usr/bin tests OTOOL: FAILED! : "$FILE1": see their, mine."
			exit 1
		else
			echo "/usr/bin tests OTOOL: SUCCESS! :"$FILE1
		fi
	done

	echo "Test OTOOL on /usr/sbin:"
	find "/usr/sbin" -type f -print0 | \
	while IFS='' read -r -d '' filename; do
		[ $? == 1 ] && exit 0;
		FILE1=$filename
		otool -t $FILE1 2> their_err  > their  
		../ft_otool $FILE1  2> mine_err  > mine
		COUNT=$(cat their mine | sort | uniq -u | wc -l)
		if [ $COUNT != 0 ]; then
			echo "/usr/sbin tests OTOOL: FAILED! : "$FILE1": see their, mine."
			exit 1
		else
			echo "/usr/bin tests OTOOL: SUCCESS! :"$FILE1
		fi
	done
fi
rm -f their mine mine_err their_err 2>/dev/null





