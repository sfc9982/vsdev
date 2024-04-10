set terminal gif animate delay 1 optimize size 1080,1080
set output 'orbit_tol-4.gif'
do for [i=0: 54000] {
  p [-1:1][-1:1] for [j=1:*] "log2.log" every ::i::i u 2*j-1:2*j w p notitle
}
unset output
