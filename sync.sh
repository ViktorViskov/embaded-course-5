CURRENT_PATH=$(pwd)

echo "Syncing files to remote server..."
rsync -av --exclude='.git' $CURRENT_PATH debian@192.168.7.2:/home/debian/dev
