#include "radix.h"

//constants
#define SIZE  (sizeof(int)*2)

static char* integral_conversion(int valint, int integral_precision,int src_radix,int dst_radix){
    int dec = 0;
    int src_integer = valint;
    int mod;
    int i1 = 0;//degree
    int dst_integral = dec;
    char digits[17] = "0123456789ABCDEF";

    //convert src_integer part from src_radix into 10
    while (src_integer != 0) {
        mod = src_integer % 10;
        src_integer /= 10;

        if (mod >= src_radix){
            throw std::exception("Radix error");
        }
        dec = dec + mod * pow(src_radix, i1++);
    }

    //convert integral part from 10 into dst_radix
    char* integral_part = new char[integral_precision+1];
    mod = 0;
    for (int i = 0; i < integral_precision; i++) {
        mod = dec % dst_radix;
        dec /= dst_radix;
        integral_part[integral_precision - i - 1] = digits[mod];
    }
    integral_part[integral_precision] = '\0';
    return integral_part;
}

static char* float_conversion(int valfloat, int src_radix,int dst_radix){
    char digits[17] = "0123456789ABCDEF";

    //convert float part from src_radix into 10
    double frc = 0, fldigit;
    int digit = 0;

    int i=1;
    while(valfloat > 0){
        digit = valfloat % 10;
        valfloat /=10;//get integral part

        if (digit >= src_radix){
            throw std::exception("Radix error");
        }

        frc += (digit / (pow(src_radix, i)));
        i++;
    }

    //convert float part from 10 int dst_radix
    char* float_part = new char[SIZE + 1];
    int digit2;

    for (int i = 0; i < SIZE; i++) {
        frc *= dst_radix;
        digit2 = frc;
        frc -= digit2;
        float_part[i] = digits[digit2];
    }
    float_part[SIZE] = '\0';
    return float_part;
}

static void reverse_str(std::string& str){
    int n = str.length();
    for (int i = 0; i < n / 2; i++)
        std::swap(str[i], str[n - i - 1]);
}

void radix_convert(std::string& src_str,std::string& buffer,int src_radix, int dst_radix) {
    if (!((src_radix > 0 && src_radix < 16) || (dst_radix > 0 && dst_radix < 16))){
        throw std::exception("Radix must be equal to 0...16");
    }

    double val = strtod(src_str.c_str(),NULL);
    if (val == 0){
        buffer = "0.0";
        return;
    }
    else if(val == HUGE_VAL || val==-HUGE_VAL){
        throw std::exception("HUGE VALUE");
    }

    int dot_position = src_str.find('.');
    int valint = val;
    std::string float_part_reversed = src_str.substr(dot_position+1,src_str.length());
    reverse_str(float_part_reversed);

    int valfloat = atoi(float_part_reversed.c_str());

    if (dot_position >= -1){//integer value
        int integral_precision = src_str.length()+1;
        char* integral_part;
        char* float_part;

        if(dot_position == -1 || dot_position == src_str.length() - 1){//xx format == xx.0 == X.
            try{
                integral_part = integral_conversion(valint,integral_precision,src_radix,dst_radix);
            } catch (std::exception& err) {
                throw err;
            }
            //to clear nills at the top
            int int_result_start;
            for (int i = 0; i < integral_precision; i++) {
                if (integral_part[i] != '0') {
                    int_result_start = i;
                    break;
                }
            }
            buffer = integral_part+int_result_start;
            delete[] integral_part;
            return;
        }
        else if(dot_position == 0){//.0 format == 0.xxx
            try{
                float_part = float_conversion(valfloat,src_radix,dst_radix);
            } catch (std::exception& err) {
                throw err;
            }

            buffer = "0.";
            buffer.append(float_part);
            delete[] float_part;
            return;
        }
        else{//x.x format
            try{
                integral_part = integral_conversion(valint,integral_precision,src_radix,dst_radix);
            } catch (std::exception& err) {
                throw err;
            }
            int int_result_start=0;
            for (int i = 0; i < integral_precision; i++) {
                if (integral_part[i] != '0') {
                    int_result_start = i;
                    break;
                }
            }

            try{
                float_part = float_conversion(valfloat,src_radix,dst_radix);
            } catch (std::exception& err) {
                throw err;
            }

            buffer = integral_part+int_result_start;
            buffer.append(".");
            buffer.append( float_part);

            delete[] integral_part;
            delete[] float_part;
        }
    }
}