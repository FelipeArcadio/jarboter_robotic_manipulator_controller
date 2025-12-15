// .c part
#include <avr/io.h>
#include "PMDtrans.h"


PMDuint16 BuildCommand(PMDuint16 OPCode, PMDAxis axis)
{
	return ((PMDuint16)((PMDuint32)OPCode | ((PMDuint32)axis << 8)));
}




PMDuint16 SendCommand(PMDAxisHandle* axis_handle, PMDuint16 OPCode)
{
        PMDuint16 buffer[1];

        buffer[0] = BuildCommand(OPCode,axis_handle->axis);

        return axis_handle->transport.SendCommand(axis_handle->transport_data, 1, buffer, 0, PMDnull);
}


PMDuint16 SendCommandWord(PMDAxisHandle* axis_handle, PMDuint16 OPCode, PMDuint16 data1)
{
        PMDuint16 buffer[2];

        buffer[0] = BuildCommand(OPCode,axis_handle->axis);
        buffer[1] = (PMDuint16)data1;

        return axis_handle->transport.SendCommand(axis_handle->transport_data, 2, buffer, 0, PMDnull);
}


PMDuint16 SendCommandWordWord(PMDAxisHandle* axis_handle, PMDuint16 OPCode, PMDuint16 data1, PMDuint16 data2)
{
        PMDuint16 buffer[3];

        buffer[0] = BuildCommand(OPCode,axis_handle->axis);
        buffer[1] = (PMDuint16)data1;
        buffer[2] = (PMDuint16)data2;

        return axis_handle->transport.SendCommand(axis_handle->transport_data, 3, buffer, 0, PMDnull);
}


PMDuint16 SendCommandLong(PMDAxisHandle* axis_handle, PMDuint16 OPCode, PMDuint32 data1)
{
        PMDuint16 buffer[3];
        PMDuint16* ptr;
		PMDuint32* temp;

        buffer[0] = BuildCommand(OPCode,axis_handle->axis);
		temp=&data1;
        ptr = (PMDuint16 *)temp;
        buffer[1]=*(ptr+1); //Write MSW first
        buffer[2]=*ptr;

        return axis_handle->transport.SendCommand(axis_handle->transport_data, 3, buffer, 0, PMDnull);
}


PMDuint16 SendCommandWordLong(PMDAxisHandle* axis_handle, PMDuint16 OPCode, PMDuint16 data1, PMDuint32 data2)
{
        PMDuint16 buffer[4];
        PMDuint16 *ptr;
		PMDuint32 *temp;

        buffer[0] = BuildCommand(OPCode,axis_handle->axis);
        buffer[1] = data1;
		temp=&data2;
        ptr = (PMDuint16 *)temp;
        buffer[2]=*(ptr+1); //Write MSW first
        buffer[3]=*ptr;

        return axis_handle->transport.SendCommand(axis_handle->transport_data, 4, buffer, 0, PMDnull);
}


PMDuint16 SendCommandGetWord(PMDAxisHandle* axis_handle, PMDuint16 OPCode, PMDuint16 *data1)
{
        PMDuint16 result = PMD_ERR_OK;
        PMDuint16 buffer[2];

        buffer[0] = BuildCommand(OPCode,axis_handle->axis);

        result = axis_handle->transport.SendCommand(axis_handle->transport_data, 1, buffer, 1, buffer+1);

        *data1 = (PMDuint16)buffer[1];

        return result;
}

PMDuint16 SendCommandGetWordWord(PMDAxisHandle* axis_handle, PMDuint16 OPCode, PMDuint16 *data1, PMDuint16 *data2)
{
        PMDuint16 result = PMD_ERR_OK;
        PMDuint16 buffer[3];

        buffer[0] = BuildCommand(OPCode,axis_handle->axis);

        result = axis_handle->transport.SendCommand(axis_handle->transport_data, 1, buffer, 2, buffer+1);

        *data1 = (PMDuint16)buffer[1];
        *data2 = (PMDuint16)buffer[2];

        return result;
}


PMDuint16 SendCommandGetLong(PMDAxisHandle* axis_handle, PMDuint16 OPCode, PMDuint32 *data1)
{
        PMDuint16 result = PMD_ERR_OK;
        PMDuint16 buffer[3];

        buffer[0] = BuildCommand(OPCode,axis_handle->axis);

        result = axis_handle->transport.SendCommand(axis_handle->transport_data, 1, buffer, 2, buffer+1);

        *data1 = ((PMDuint32)buffer[1])<<16 | ((PMDuint32)buffer[2]);

        return result;
}

PMDuint16 SendCommandWordGetWord(PMDAxisHandle* axis_handle, PMDuint16 OPCode, PMDuint16 data1, PMDuint16 *data2)
{
        PMDuint16 result = PMD_ERR_OK;
        PMDuint16 buffer[3];

        buffer[0] = BuildCommand(OPCode,axis_handle->axis);
        buffer[1] = (PMDuint16)data1;

        result = axis_handle->transport.SendCommand(axis_handle->transport_data, 2, buffer, 1, buffer+2);

        *data2 = (PMDuint16)buffer[2];

        return result;
}

PMDuint16 SendCommandWordGetLong(PMDAxisHandle* axis_handle, PMDuint16 OPCode, PMDuint16 data1, PMDuint32 *data2)
{
        PMDuint16 result = PMD_ERR_OK;
        PMDuint16 buffer[4];

        buffer[0] = BuildCommand(OPCode,axis_handle->axis);
        buffer[1] = (PMDuint16)data1;

        result = axis_handle->transport.SendCommand(axis_handle->transport_data, 2, buffer, 2, buffer+2);

        *data2 = ((PMDuint32)buffer[2])<<16 | ((PMDuint32)buffer[3]);

        return result;
}
