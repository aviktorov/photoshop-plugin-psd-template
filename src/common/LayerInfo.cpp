#include "LayerInfo.h"

#include "PIUGet.h"
#include "PITerminology.h"
#include "Utils.h"

SPErr LayerInfo::load(const std::wstring &name) {
	Auto_Ref layer_ref;
	SPErr error = Utils::PutNameWString(layer_ref.get(),classLayer,name);
	if(error) return error;
	
	Auto_Desc layer;
	error = sPSActionControl->Get(&layer,layer_ref.get());
	if(error) return error;
	
	Auto_Desc text_layer;
	DescriptorClassID text_layer_id;
	error = sPSActionDescriptor->GetObjectA(layer.get(),keyText,&text_layer_id,&text_layer);
	if(error) return error;
	
	if(text_layer_id != classTextLayer) return kBadParameterErr;
	
	Auto_List paragraph_list;
	error = PIUGetSingleItemFromDescriptor(text_layer.get(),keyParagraphStyleRange,&paragraph_list,NULL);
	if(error) return error;
	
	Auto_Desc paragraph;
	DescriptorClassID paragraph_id;
	error = sPSActionList->GetObjectA(paragraph_list.get(),0,&paragraph_id,&paragraph);
	if(error) return error;
	
	Auto_List text_list;
	error = PIUGetSingleItemFromDescriptor(text_layer.get(),keyTextStyleRange,&text_list,NULL);
	if(error) return error;
	
	Auto_Desc text;
	DescriptorClassID text_id;
	error = sPSActionList->GetObjectA(text_list.get(),0,&text_id,&text);
	if(error) return error;
	
	return error;
}

SPErr LayerInfo::save(PIActionDescriptor &data) const {
	SPErr error = kNoErr;
	
	return error;
}
