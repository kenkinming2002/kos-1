.global local_lock
.global local_unlock
.global spin_lock
.global spin_unlock

local_lock:
  pushfq
  pop 0(%rdi)
  ret

local_unlock:
  push 0(%rdi)
  popfq
  ret

spin_lock:
0:
  pushfq
  pop 0(%rdi)
  lock btsq $0, 8(%rdi)
  jc 0b
  ret

spin_unlock:
  lock btrq $0, 8(%rdi)
  push 0(%rdi)
  popfq
  ret
