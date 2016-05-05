#!/bin/sh
# Reads all relevant memory sections from the device and prints it to stdout an automatically
# generated file.



# ---  S E T T I N G S  --- #
AVRDUDE_CMD="avrdude -c atmelice_pdi -p x128a4u -U "
OUTPUT_FILE="uMIDI_dump_`date +%F_%H%M%S`.txt"



# ---  M A I N   S C R I P T  --- #

# Read flash (including bootloader!) and extract software version string
TEMP_FILE=`mktemp`
$AVRDUDE_CMD flash:r:-:r  2>/dev/null >$TEMP_FILE
echo "##############################"              | tee -a $OUTPUT_FILE
strings $TEMP_FILE | grep -E '[0-9]{4}-'           | tee -a $OUTPUT_FILE
echo "##############################"              | tee -a $OUTPUT_FILE
echo "## FLASH ##"                                 | tee -a $OUTPUT_FILE
cat $TEMP_FILE | hexdump -C                        | tee -a $OUTPUT_FILE
rm $TEMP_FILE

# Read EEPROM
echo                                               | tee -a $OUTPUT_FILE
echo "## EEPROM ##"                                | tee -a $OUTPUT_FILE
$AVRDUDE_CMD eeprom:r:-:r 2>/dev/null | hexdump -C | tee -a $OUTPUT_FILE

# Read fuses
echo                                               | tee -a $OUTPUT_FILE
echo "## FUSE1 ##"                                 | tee -a $OUTPUT_FILE
$AVRDUDE_CMD fuse1:r:-:h  2>/dev/null              | tee -a $OUTPUT_FILE
echo "## FUSE2 ##"                                 | tee -a $OUTPUT_FILE
$AVRDUDE_CMD fuse2:r:-:h  2>/dev/null              | tee -a $OUTPUT_FILE
echo "## FUSE4 ##"                                 | tee -a $OUTPUT_FILE
$AVRDUDE_CMD fuse4:r:-:h  2>/dev/null              | tee -a $OUTPUT_FILE
echo "## FUSE5 ##"                                 | tee -a $OUTPUT_FILE
$AVRDUDE_CMD fuse5:r:-:h  2>/dev/null              | tee -a $OUTPUT_FILE
