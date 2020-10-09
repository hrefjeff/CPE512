# 2020-09-10

## Traditional Scientific Applications

1. Weather
1. Climate
1. Mechanical Devices
1. Circuits

n^4 practical problem
n^n unsolvable

## Moore's Law

### Clock speeds double every 18 months to 3-years

Therefore you can expect performance to double every 2 years - NO LONGER TRUE
Number of transistors that can be placed on a die doubles every 18 months to 3 years - SLOWING DOWN NOW BUT NOT PROBABLY TRUE FOR A WHILE LONGER

10^9 - gigahertz
10^10 - terahertz

# 2020-09-29

Parameters of blocking receive

MPI_Send (buf, count, datatype, src, tag, comm, status)

**buf** - address of receive buffer
**count** - max number of items to receive
**datatype** - datatype of each item
**src** - rank of source process (where it came from)
**tag** - message tag
**comm** - communicator (usually will be an mpi process)
**status** - status after operation

## Example
To send an integer x from process 0 to process 1

```C++
int myrank,msgtag=0; MPI_Status status;
MPI_Comm_rank(MPI_COMM_WORLD, &myrank); //find rank

if (myrank == 0) {
  int x;
  x = generate_int_data_on_rank_0_process();
} else if (myrank == 1) {
  int y;
  MPI_Recv(&y, 1, MPI_INT, 0, msgtag, MPI_COMM_WORLD, &status);
  use_int_data_on_rank_1_process(y);
}
```
