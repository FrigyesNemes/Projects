for i in `cat ../../../../E_CM_900_GeV_beta_star_100_m/General_settings/List_of_runs.txt` ; do cat ../../Project_files/Per_run/Left_bottom_right_top_4_sigma_all_root_files_to_define_cuts_template.prj | sed "s/NNNN/$i/g" > ../../Project_files/Per_run/Left_bottom_right_top_4_sigma_all_root_files_to_define_cuts_run_$i.prj ; done
for i in `cat ../../../../E_CM_900_GeV_beta_star_100_m/General_settings/List_of_runs.txt` ; do cat ../../Project_files/Per_run/Left_top_right_bottom_4_sigma_all_root_files_to_define_cuts_template.prj | sed "s/NNNN/$i/g" > ../../Project_files/Per_run/Left_top_right_bottom_4_sigma_all_root_files_to_define_cuts_run_$i.prj ; done