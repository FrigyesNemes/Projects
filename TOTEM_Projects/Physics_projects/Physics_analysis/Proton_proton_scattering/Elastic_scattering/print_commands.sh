for i in `ls Projects/2023/E_CM_900_GeV_beta_star_100_m/Elastic_analysis/Project_files/Per_run/Left_bottom_right_top_4_sigma_all_root_files_to_define_cuts_* | grep run_ | sort` ; do echo "./analyse "$i ; done
for i in `ls Projects/2023/E_CM_900_GeV_beta_star_100_m/Elastic_analysis/Project_files/Per_run/Left_top_right_bottom_4_sigma_all_root_files_to_define_cuts_* | grep run_ | sort` ; do echo "./analyse "$i ; done