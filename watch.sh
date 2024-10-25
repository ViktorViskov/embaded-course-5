inotifywait -m -e close_write $1 |
while read -r directory events filename; do
    bash sync.sh
done