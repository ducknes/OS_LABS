echo "check '$1'"

if [ -f "$1" ]; then
	echo "file exists"
else
	echo "file not exists";
	cat > "$1"
	echo "file '$1' created"
fi
