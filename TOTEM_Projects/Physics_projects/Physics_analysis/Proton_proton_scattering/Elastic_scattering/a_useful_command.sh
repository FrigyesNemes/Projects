for i in `cat cuts.txt` ; do grep begin.*$i -A11  Projects/2023/E_CM_900_GeV_beta_star_100_m/Elastic_analysis/Cuts/Fill_7291/Cuts_test_left_bottom_right_top_4_sigma.prj | tail -n12 ; echo  ; done  | grep "number_of_sigmas.*4" -B10 -A1