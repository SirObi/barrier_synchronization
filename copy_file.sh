#scp test_combined_barrier_diss 172.31.35.230:/home/pcpc/barrier_synchronization/

while read -r line; do for host in $line; do ssh $host "mkdir -p ~/barrier_synchronization"; done; done < worker_nodes.txt
while read -r line; do for host in $line; do scp $1 "$host:/home/pcpc/barrier_synchronization/"; done; done < worker_nodes.txt
