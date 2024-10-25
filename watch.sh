inotifywait -m -r -e close_write ../ |
while read -r directory events filename; do
    bash sync.sh
done